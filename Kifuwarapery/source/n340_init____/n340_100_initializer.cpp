#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_400_bitboardAll.hpp"
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
			UtilBitboard::SetBit(&result,sq);
	}
	result &= ~(BitboardMask::GetRankMask<Rank1>() | BitboardMask::GetRankMask<Rank9>() | BitboardMask::GetFileMask<FileA>() | BitboardMask::GetFileMask<FileI>());
	UtilBitboard::ClearBit(&result,square);

	return result;
}

// square のマスにおける、障害物を調べる必要がある場所を Bitboard で返す。
// lance の前方だけを調べれば良さそうだけど、Rank8 ~ Rank2 の状態をそのまま index に使いたいので、
// 縦方向全て(端を除く)の occupied を全て調べる。
Bitboard Initializer::LanceBlockMask(const Square square) {
	return BitboardMask::GetSquareFileMask(square) & ~(BitboardMask::GetRankMask<Rank1>() | BitboardMask::GetRankMask<Rank9>());
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
			UtilBitboard::SetBit(&result, sq);
			if (UtilBitboard::IsSet(&occupied, sq))
				break;
		}
	}

	return result;
}

// lance の利きを返す。
// 香車の利きは常にこれを使っても良いけど、もう少し速くする為に、テーブル化する為だけに使う。
// occupied  障害物があるマスが 1 の bitboard
Bitboard Initializer::LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied) {
	return UtilBitboard::RookAttack(&occupied, square) & BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(square));
}

// index, bits の情報を元にして、occupied の 1 のbit を いくつか 0 にする。
// index の値を, occupied の 1のbit の位置に変換する。
// index   [0, 1<<bits) の範囲のindex
// bits    bit size
// blockMask   利きのあるマスが 1 のbitboard
// result  occupied
Bitboard Initializer::IndexToOccupied(const int index, const int bits, const Bitboard& blockMask) {
	Bitboard tmpBlockMask = blockMask;
	Bitboard result = Bitboard::AllZeroBB();
	for (int i = 0; i < bits; ++i) {
		const Square sq = tmpBlockMask.FirstOneFromI9();
		if (index & (1 << i))
			UtilBitboard::SetBit(&result,sq);
	}
	return result;
}

void Initializer::InitAttacks(const bool isBishop)
{
	auto* attacks = (isBishop ? UtilBitboard::m_bishopAttack : UtilBitboard::m_rookAttack);
	auto* attackIndex = (isBishop ? UtilBitboard::m_bishopAttackIndex : ConfigBits::m_rookAttackIndex);
	auto* blockMask = (isBishop ? UtilBitboard::m_bishopBlockMask : UtilBitboard::m_rookBlockMask);
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
			const Bitboard occupied = this->IndexToOccupied(i, num1s, blockMask[sq]);
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

// LanceBlockMask, g_lanceAttack の値を設定する。
void Initializer::InitLanceAttacks() {
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			const Bitboard blockMask = LanceBlockMask(sq);
			//const int num1s = blockMask.popCount(); // 常に 7
			const int num1s = 7;
			assert(num1s == blockMask.PopCount());
			for (int i = 0; i < (1 << num1s); ++i) {
				Bitboard occupied = IndexToOccupied(i, num1s, blockMask);
				UtilBitboard::m_lanceAttack[c][sq][i] = LanceAttackCalc(c, sq, occupied);
			}
		}
	}
}

void Initializer::InitKingAttacks() {
	for (Square sq = I9; sq < SquareNum; ++sq)
		UtilBitboard::m_kingAttack[sq] = UtilBitboard::RookAttack(&Bitboard::AllOneBB(),sq) | UtilBitboard::BishopAttack(&Bitboard::AllOneBB(),sq);
}

void Initializer::InitGoldAttacks() {
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			UtilBitboard::m_goldAttack[c][sq] = (UtilBitboard::KingAttack(sq) & BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(sq))) | UtilBitboard::RookAttack(&Bitboard::AllOneBB(),sq);
}

void Initializer::InitSilverAttacks() {
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			UtilBitboard::m_silverAttack[c][sq] = (UtilBitboard::KingAttack(sq) & BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(sq))) | UtilBitboard::BishopAttack(&Bitboard::AllOneBB(),sq);
}

void Initializer::InitKnightAttacks() {
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			UtilBitboard::m_knightAttack[c][sq] = Bitboard::AllZeroBB();
			const Bitboard bb = UtilBitboard::PawnAttack(c, sq);
			if (bb.IsNot0())
				UtilBitboard::m_knightAttack[c][sq] = UtilBitboard::BishopStepAttacks(bb.ConstFirstOneFromI9()) & BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(sq));
		}
	}
}

void Initializer::InitPawnAttacks() {
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			UtilBitboard::m_pawnAttack[c][sq] = UtilBitboard::SilverAttack(c, sq) ^ UtilBitboard::BishopAttack(&Bitboard::AllOneBB(),sq);
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
		UtilBitboard::m_rookAttackToEdge[sq] = UtilBitboard::RookAttack(&Bitboard::AllZeroBB(),sq);
		UtilBitboard::m_bishopAttackToEdge[sq] = UtilBitboard::BishopAttack(&Bitboard::AllZeroBB(),sq);
		UtilBitboard::m_lanceAttackToEdge[Black][sq] = UtilBitboard::LanceAttack(&Bitboard::AllZeroBB(), Black, sq);
		UtilBitboard::m_lanceAttackToEdge[White][sq] = UtilBitboard::LanceAttack(&Bitboard::AllZeroBB(), White, sq);
	}
}

void Initializer::InitBetweenBB() {
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			UtilBitboard::m_betweenBB[sq1][sq2] = Bitboard::AllZeroBB();
			if (sq1 == sq2) continue;
			const Direction direc = UtilSquare::GetSquareRelation(sq1, sq2);
			if (direc & DirecCross)
				UtilBitboard::m_betweenBB[sq1][sq2] = UtilBitboard::RookAttack(&UtilBitboard::SetMaskBB(sq2),sq1) & UtilBitboard::RookAttack(&UtilBitboard::SetMaskBB(sq1),sq2);
			else if (direc & DirecDiag)
				UtilBitboard::m_betweenBB[sq1][sq2] = UtilBitboard::BishopAttack(&UtilBitboard::SetMaskBB(sq2),sq1) & UtilBitboard::BishopAttack(&UtilBitboard::SetMaskBB(sq1),sq2);
		}
	}
}

void Initializer::InitCheckTable() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			UtilBitboard::m_goldCheckTable[c][sq] = Bitboard::AllZeroBB();
			Bitboard checkBB = UtilBitboard::GoldAttack(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				UtilBitboard::m_goldCheckTable[c][sq] |= UtilBitboard::GoldAttack(opp, checkSq);
			}
			UtilBitboard::m_goldCheckTable[c][sq].AndEqualNot(UtilBitboard::SetMaskBB(sq) | UtilBitboard::GoldAttack(opp, sq));
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			UtilBitboard::m_silverCheckTable[c][sq] = Bitboard::AllZeroBB();

			Bitboard checkBB = UtilBitboard::SilverAttack(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				UtilBitboard::m_silverCheckTable[c][sq] |= UtilBitboard::SilverAttack(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			checkBB = UtilBitboard::GoldAttack(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				// 移動元が敵陣である位置なら、金に成って王手出来る。
				UtilBitboard::m_silverCheckTable[c][sq] |= (UtilBitboard::SilverAttack(opp, checkSq) & TRank789BB);
			}

			const Bitboard TRank6BB = (c == Black ? BitboardMask::GetRankMask<Rank6>() : BitboardMask::GetRankMask<Rank4>());
			// 移動先が3段目で、4段目に移動したときも、成ることが出来る。
			checkBB = UtilBitboard::GoldAttack(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				UtilBitboard::m_silverCheckTable[c][sq] |= (UtilBitboard::SilverAttack(opp, checkSq) & TRank6BB);
			}
			UtilBitboard::m_silverCheckTable[c][sq].AndEqualNot(UtilBitboard::SetMaskBB(sq) | UtilBitboard::SilverAttack(opp, sq));
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			UtilBitboard::m_knightCheckTable[c][sq] = Bitboard::AllZeroBB();

			Bitboard checkBB = UtilBitboard::KnightAttack(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				UtilBitboard::m_knightCheckTable[c][sq] |= UtilBitboard::KnightAttack(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			checkBB = UtilBitboard::GoldAttack(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				UtilBitboard::m_knightCheckTable[c][sq] |= UtilBitboard::KnightAttack(opp, checkSq);
			}
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			UtilBitboard::m_lanceCheckTable[c][sq] = UtilBitboard::LanceAttackToEdge(opp, sq);

			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			Bitboard checkBB = UtilBitboard::GoldAttack(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				UtilBitboard::m_lanceCheckTable[c][sq] |= UtilBitboard::LanceAttackToEdge(opp, checkSq);
			}
			UtilBitboard::m_lanceCheckTable[c][sq].AndEqualNot(UtilBitboard::SetMaskBB(sq) | UtilBitboard::PawnAttack(opp, sq));
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
				if (UtilBitboard::IsSet(&UtilBitboard::KnightAttack(Black, sq0),sq1) || UtilBitboard::IsSet(&UtilBitboard::KnightAttack(White, sq0),sq1))
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
	this->InitKingAttacks();

	SYNCCOUT << "(^q^)I4: initGoldAttacks!" << SYNCENDL;
	this->InitGoldAttacks();

	SYNCCOUT << "(^q^)I5: initSilverAttacks!" << SYNCENDL;
	this->InitSilverAttacks();

	SYNCCOUT << "(^q^)I6: initPawnAttacks!" << SYNCENDL;
	this->InitPawnAttacks();

	SYNCCOUT << "(^q^)I7: initKnightAttacks!" << SYNCENDL;
	this->InitKnightAttacks();

	SYNCCOUT << "(^q^)I8: initLanceAttacks!" << SYNCENDL;
	this->InitLanceAttacks();

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
		occupied[i] = IndexToOccupied(i, num1s, mask);
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
