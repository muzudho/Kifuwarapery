#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_600_bitboardAll.hpp"
#include "../../header/n260_evaluate/n260_700_evaluate.hpp"
#include "../../header/n280_move____/n280_200_mt64bit.hpp"
#include "../../header/n300_book____/n300_100_book.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n340_init____/n340_100_initializer.hpp"


// square のマスにおける、障害物を調べる必要がある場所を調べて Bitboard で返す。
Bitboard Initializer::RookBlockMaskCalc(const Square square) {
	Bitboard result = BitboardMask::GetSquareFileMask(square) ^ BitboardMask::GetSquareRankMask(square);
	if (UtilSquare::ToFile(square) != FileA) { result &= ~BitboardMask::GetFileMask<FileA>(); }
	if (UtilSquare::ToFile(square) != FileI) { result &= ~BitboardMask::GetFileMask<FileI>(); }
	if (UtilSquare::ToRank(square) != Rank1) { result &= ~BitboardMask::GetRankMask<Rank1>(); }
	if (UtilSquare::ToRank(square) != Rank9) { result &= ~BitboardMask::GetRankMask<Rank9>(); }
	return result;
}

// square のマスにおける、障害物を調べる必要がある場所を調べて Bitboard で返す。
Bitboard Initializer::BishopBlockMaskCalc(const Square square) {
	const Rank rank = UtilSquare::ToRank(square);
	const File file = UtilSquare::ToFile(square);
	Bitboard result = Bitboard::AllZeroBB();
	for (Square sq = I9; sq < SquareNum; ++sq) {
		const Rank r = UtilSquare::ToRank(sq);
		const File f = UtilSquare::ToFile(sq);
		if (abs(rank - r) == abs(file - f))
			g_setMaskBb.SetBit(&result,sq);
	}
	result &= ~(BitboardMask::GetRankMask<Rank1>() | BitboardMask::GetRankMask<Rank9>() | BitboardMask::GetFileMask<FileA>() | BitboardMask::GetFileMask<FileI>());
	g_setMaskBb.ClearBit(&result,square);

	return result;
}


// Rook or Bishop の利きの範囲を調べて bitboard で返す。
// occupied  障害物があるマスが 1 の bitboard
Bitboard Initializer::AttackCalc(const Square square, const Bitboard& occupied, const bool isBishop) {
	const SquareDelta deltaArray[2][4] = { { DeltaN, DeltaS, DeltaE, DeltaW },{ DeltaNE, DeltaSE, DeltaSW, DeltaNW } };
	Bitboard result = Bitboard::AllZeroBB();
	for (SquareDelta delta : deltaArray[isBishop]) {
		for (Square sq = square + delta;
		UtilSquare::ContainsOf(sq) && abs(UtilSquare::ToRank(sq - delta) - UtilSquare::ToRank(sq)) <= 1;
			sq += delta)
		{
			g_setMaskBb.SetBit(&result, sq);
			if (g_setMaskBb.IsSet(&occupied, sq))
				break;
		}
	}

	return result;
}

void Initializer::InitAttacks(const bool isBishop)
{
	auto* attacks = (isBishop ? g_bishopAttackBb.m_controllBb : g_rookAttackBb.m_controllBb);
	auto* attackIndex = (isBishop ? g_bishopAttackBb.m_controllBbIndex : ConfigBits::m_rookAttackIndex);
	auto* blockMask = (isBishop ? g_bishopAttackBb.m_bishopBlockMask : g_rookAttackBb.m_rookBlockMask);
	auto* shift = (isBishop ? ConfigBits::m_bishopShiftBits : ConfigBits::m_rookShiftBits);
#if defined HAVE_BMI2
#else
	auto* magic = (isBishop ? ConfigBits::m_bishopMagic : ConfigBits::m_rookMagic);
#endif
	int index = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		blockMask[sq] = (isBishop ? BishopBlockMaskCalc(sq) : RookBlockMaskCalc(sq));
		attackIndex[sq] = index;

		const int num1s = (isBishop ? ConfigBits::m_bishopBlockBits[sq] : ConfigBits::m_rookBlockBits[sq]);
		for (int i = 0; i < (1 << num1s); ++i)
		{
			const Bitboard occupied = g_setMaskBb.IndexToOccupied(i, num1s, blockMask[sq]);
#if defined HAVE_BMI2
			attacks[index + (occupied & blockMask[sq]).OccupiedToIndex( blockMask[sq])] = AttackCalc(sq, occupied, isBishop);
#else
			attacks[index + occupied.OccupiedToIndex( magic[sq], shift[sq])] =
				this->AttackCalc(sq, occupied, isBishop);
#endif
		}
		index += 1 << (64 - shift[sq]);
	}
}

void Initializer::InitSquareRelation() {
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		const File file1 = UtilSquare::ToFile(sq1);
		const Rank rank1 = UtilSquare::ToRank(sq1);
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			const File file2 = UtilSquare::ToFile(sq2);
			const Rank rank2 = UtilSquare::ToRank(sq2);
			g_squareRelation[sq1][sq2] = DirecMisc;
			if (sq1 == sq2) continue;

			if (file1 == file2)
				g_squareRelation[sq1][sq2] = DirecFile;
			else if (rank1 == rank2)
				g_squareRelation[sq1][sq2] = DirecRank;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file1 - file2))
				g_squareRelation[sq1][sq2] = DirecDiagNESW;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file2 - file1))
				g_squareRelation[sq1][sq2] = DirecDiagNWSE;
		}
	}
}

// 障害物が無いときの利きの Bitboard
// g_rookAttack, g_bishopAttack, g_lanceAttack を設定してから、この関数を呼ぶこと。
void Initializer::InitAttackToEdge() {
	for (Square sq = I9; sq < SquareNum; ++sq) {
		g_rookAttackBb.m_controllBbToEdge[sq] = g_rookAttackBb.GetControllBb(&Bitboard::AllZeroBB(),sq);
		g_bishopAttackBb.m_controllBbToEdge[sq] = g_bishopAttackBb.BishopAttack(&Bitboard::AllZeroBB(),sq);
		g_lanceAttackBb.m_controllBbToEdge[Black][sq] = g_lanceAttackBb.GetControllBb(&Bitboard::AllZeroBB(), Black, sq);
		g_lanceAttackBb.m_controllBbToEdge[White][sq] = g_lanceAttackBb.GetControllBb(&Bitboard::AllZeroBB(), White, sq);
	}
}

void Initializer::InitBetweenBB() {
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			g_betweenBb.m_betweenBB[sq1][sq2] = Bitboard::AllZeroBB();
			if (sq1 == sq2) continue;
			const Direction direc = UtilSquare::GetSquareRelation(sq1, sq2);
			if (direc & DirecCross)
				g_betweenBb.m_betweenBB[sq1][sq2] = g_rookAttackBb.GetControllBb(&g_setMaskBb.GetSetMaskBb(sq2),sq1) & g_rookAttackBb.GetControllBb(&g_setMaskBb.GetSetMaskBb(sq1),sq2);
			else if (direc & DirecDiag)
				g_betweenBb.m_betweenBB[sq1][sq2] = g_bishopAttackBb.BishopAttack(&g_setMaskBb.GetSetMaskBb(sq2),sq1) & g_bishopAttackBb.BishopAttack(&g_setMaskBb.GetSetMaskBb(sq1),sq2);
		}
	}
}

void Initializer::InitCheckTable() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_goldAttackBb.m_goldCheckTable[c][sq] = Bitboard::AllZeroBB();
			Bitboard checkBB = g_goldAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_goldAttackBb.m_goldCheckTable[c][sq] |= g_goldAttackBb.GetControllBb(opp, checkSq);
			}
			g_goldAttackBb.m_goldCheckTable[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_goldAttackBb.GetControllBb(opp, sq));
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_silverAttackBb.m_silverCheckTable[c][sq] = Bitboard::AllZeroBB();

			Bitboard checkBB = g_silverAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_silverAttackBb.m_silverCheckTable[c][sq] |= g_silverAttackBb.GetControllBb(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			checkBB = g_goldAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				// 移動元が敵陣である位置なら、金に成って王手出来る。
				g_silverAttackBb.m_silverCheckTable[c][sq] |= (g_silverAttackBb.GetControllBb(opp, checkSq) & TRank789BB);
			}

			const Bitboard TRank6BB = (c == Black ? BitboardMask::GetRankMask<Rank6>() : BitboardMask::GetRankMask<Rank4>());
			// 移動先が3段目で、4段目に移動したときも、成ることが出来る。
			checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_silverAttackBb.m_silverCheckTable[c][sq] |= (g_silverAttackBb.GetControllBb(opp, checkSq) & TRank6BB);
			}
			g_silverAttackBb.m_silverCheckTable[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_silverAttackBb.GetControllBb(opp, sq));
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_knightAttackBb.m_knightCheckTable[c][sq] = Bitboard::AllZeroBB();

			Bitboard checkBB = g_knightAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_knightAttackBb.m_knightCheckTable[c][sq] |= g_knightAttackBb.GetControllBb(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_knightAttackBb.m_knightCheckTable[c][sq] |= g_knightAttackBb.GetControllBb(opp, checkSq);
			}
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_lanceAttackBb.m_lanceCheckTable[c][sq] = g_lanceAttackBb.GetControllBbToEdge(opp, sq);

			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			Bitboard checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_lanceAttackBb.m_lanceCheckTable[c][sq] |= g_lanceAttackBb.GetControllBbToEdge(opp, checkSq);
			}
			g_lanceAttackBb.m_lanceCheckTable[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_pawnAttackBb.GetControllBb(opp, sq));
		}
	}
}

void Initializer::InitSquareDistance() {
	for (Square sq0 = I9; sq0 < SquareNum; ++sq0) {
		for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
			switch (UtilSquare::GetSquareRelation(sq0, sq1)) {
			case DirecMisc:
				// DirecMisc な関係は全て距離 1 にしてもKPE学習には問題無いんだけれど。
				g_squareDistance[sq0][sq1] = 0;
				if (g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(Black, sq0),sq1) || g_setMaskBb.IsSet(&g_knightAttackBb.GetControllBb(White, sq0),sq1))
					g_squareDistance[sq0][sq1] = 1;
				break;
			case DirecFile:
				g_squareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaN));
				break;
			case DirecRank:
				g_squareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaE));
				break;
			case DirecDiagNESW:
				g_squareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaNE));
				break;
			case DirecDiagNWSE:
				g_squareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaNW));
				break;
			default: UNREACHABLE;
			}
		}
	}
}

void Initializer::InitTable() {

#ifndef SKIP_LONG_TIME_EVAL
	SYNCCOUT << "(^q^)I1: SKIP! (long time)initAttacks!" << SYNCENDL;
	this->InitAttacks(false);
#endif

	SYNCCOUT << "(^q^)I2: initAttacks!" << SYNCENDL;
	this->InitAttacks(true);

	SYNCCOUT << "(^q^)I3: initKingAttacks!" << SYNCENDL;
	g_kingAttackBb.Initialize();

	SYNCCOUT << "(^q^)I4: initGoldAttacks!" << SYNCENDL;
	g_goldAttackBb.Initialize();

	SYNCCOUT << "(^q^)I5: initSilverAttacks!" << SYNCENDL;
	g_silverAttackBb.Initialize();

	SYNCCOUT << "(^q^)I6: initPawnAttacks!" << SYNCENDL;
	g_pawnAttackBb.Initialize();

	SYNCCOUT << "(^q^)I7: initKnightAttacks!" << SYNCENDL;
	g_knightAttackBb.Initialize();

	SYNCCOUT << "(^q^)I8: initLanceAttacks!" << SYNCENDL;
	g_lanceAttackBb.Initialize();

	SYNCCOUT << "(^q^)I9: initSquareRelation!" << SYNCENDL;
	this->InitSquareRelation();

	SYNCCOUT << "(^q^)I10: initAttackToEdge!" << SYNCENDL;
	this->InitAttackToEdge();

	SYNCCOUT << "(^q^)I11: initBetweenBB!" << SYNCENDL;
	this->InitBetweenBB();

	SYNCCOUT << "(^q^)I12: initCheckTable!" << SYNCENDL;
	this->InitCheckTable();

	SYNCCOUT << "(^q^)I13: initSquareDistance!" << SYNCENDL;
	this->InitSquareDistance();

	SYNCCOUT << "(^q^)I14: Book::init!" << SYNCENDL;
	Book::Init();

	SYNCCOUT << "(^q^)I15: initSearchTable!" << SYNCENDL;
	InitSearchTable();
}

#if defined FIND_MAGIC
// square の位置の rook, bishop それぞれのMagic Bitboard に使用するマジックナンバーを見つける。
// isBishop  : true なら bishop, false なら rook のマジックナンバーを見つける。
u64 Initializer::findMagic(const Square square, const bool isBishop) {
	Bitboard occupied[1<<14];
	Bitboard attack[1<<14];
	Bitboard attackUsed[1<<14];
	Bitboard mask = (isBishop ? BishopBlockMaskCalc(square) : RookBlockMaskCalc(square));
	int num1s = (isBishop ? g_bishopBlockBits[square] : g_rookBlockBits[square]);

	// n bit の全ての数字 (利きのあるマスの全ての 0 or 1 の組み合わせ)
	for (int i = 0; i < (1 << num1s); ++i) {
		occupied[i] = g_setMaskBb.IndexToOccupied(i, num1s, mask);
		attack[i] = AttackCalc(square, occupied[i], isBishop);
	}

	for (u64 k = 0; k < UINT64_C(100000000); ++k) {
		const u64 magic = g_mt64bit.GetRandomFewBits();
		bool fail = false;

		// これは無くても良いけど、少しマジックナンバーが見つかるのが早くなるはず。
		if (count1s((mask.MergeP() * magic) & UINT64_C(0xfff0000000000000)) < 6)
			continue;

		std::fill(std::begin(attackUsed), std::IsEnd(attackUsed), AllZeroBB());

		for (int i = 0; !fail && i < (1 << num1s); ++i) {
			const int shiftBits = (isBishop ? g_bishopShiftBits[square] : g_rookShiftBits[square]);
			const u64 index = OccupiedToIndex(occupied[i], magic, shiftBits);
			if      (attackUsed[index] == AllZeroBB())
				attackUsed[index] = attack[i];
			else if (attackUsed[index] != attack[i])
				fail = true;
		}
		if (!fail)
			return magic;
	}

	std::cout << "/***Failed***/\t";
	return 0;
}
#endif // #if defined FIND_MAGIC
