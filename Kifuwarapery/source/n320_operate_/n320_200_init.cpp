#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_400_bitboardAll.hpp"
#include "../../header/n260_evaluate/n260_700_evaluate.hpp"
#include "../../header/n280_move____/n280_200_mt64bit.hpp"
#include "../../header/n320_operate_/n320_100_book.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n320_operate_/n320_200_init.hpp"


// square のマスにおける、障害物を調べる必要がある場所を調べて Bitboard で返す。
Bitboard Initializer::rookBlockMaskCalc(const Square square) {
	Bitboard result = squareFileMask(square) ^ squareRankMask(square);
	if (Util_Square::MakeFile(square) != FileA) { result &= ~fileMask<FileA>(); }
	if (Util_Square::MakeFile(square) != FileI) { result &= ~fileMask<FileI>(); }
	if (Util_Square::MakeRank(square) != Rank1) { result &= ~rankMask<Rank1>(); }
	if (Util_Square::MakeRank(square) != Rank9) { result &= ~rankMask<Rank9>(); }
	return result;
}

// square のマスにおける、障害物を調べる必要がある場所を調べて Bitboard で返す。
Bitboard Initializer::bishopBlockMaskCalc(const Square square) {
	const Rank rank = Util_Square::MakeRank(square);
	const File file = Util_Square::MakeFile(square);
	Bitboard result = Bitboard::allZeroBB();
	for (Square sq = I9; sq < SquareNum; ++sq) {
		const Rank r = Util_Square::MakeRank(sq);
		const File f = Util_Square::MakeFile(sq);
		if (abs(rank - r) == abs(file - f))
			result.setBit(sq);
	}
	result &= ~(rankMask<Rank1>() | rankMask<Rank9>() | fileMask<FileA>() | fileMask<FileI>());
	result.clearBit(square);

	return result;
}

// square のマスにおける、障害物を調べる必要がある場所を Bitboard で返す。
// lance の前方だけを調べれば良さそうだけど、Rank8 ~ Rank2 の状態をそのまま index に使いたいので、
// 縦方向全て(端を除く)の occupied を全て調べる。
Bitboard Initializer::lanceBlockMask(const Square square) {
	return squareFileMask(square) & ~(rankMask<Rank1>() | rankMask<Rank9>());
}

// Rook or Bishop の利きの範囲を調べて bitboard で返す。
// occupied  障害物があるマスが 1 の bitboard
Bitboard Initializer::attackCalc(const Square square, const Bitboard& occupied, const bool isBishop) {
	const SquareDelta deltaArray[2][4] = {
		{ DeltaN, DeltaS, DeltaE, DeltaW },// 飛車
		{ DeltaNE, DeltaSE, DeltaSW, DeltaNW }// 角
	};
	Bitboard result = Bitboard::allZeroBB();
	for (SquareDelta delta : deltaArray[isBishop]) {
		for (Square sq = square + delta;
		Util_Square::IsInSquare(sq) && abs(Util_Square::MakeRank(sq - delta) - Util_Square::MakeRank(sq)) <= 1;
			sq += delta)
		{
			result.setBit(sq);
			if (occupied.isSet(sq))
			{
				break;
			}
		}
	}

	return result;
}

// lance の利きを返す。
// 香車の利きは常にこれを使っても良いけど、もう少し速くする為に、テーブル化する為だけに使う。
// occupied  障害物があるマスが 1 の bitboard
Bitboard Initializer::lanceAttackCalc(const Color c, const Square square, const Bitboard& occupied) {
	return occupied.rookAttack(square) & inFrontMask(c, Util_Square::MakeRank(square));
}

// index, bits の情報を元にして、occupied の 1 のbit を いくつか 0 にする。
// index の値を, occupied の 1のbit の位置に変換する。
// index   [0, 1<<bits) の範囲のindex
// bits    bit size
// blockMask   利きのあるマスが 1 のbitboard
// result  occupied
Bitboard Initializer::indexToOccupied(const int index, const int bits, const Bitboard& blockMask) {
	Bitboard tmpBlockMask = blockMask;
	Bitboard result = Bitboard::allZeroBB();
	for (int i = 0; i < bits; ++i) {
		const Square sq = tmpBlockMask.firstOneFromI9();
		if (index & (1 << i))
			result.setBit(sq);
	}
	return result;
}

// 角か、飛車かで分かれている？
void Initializer::initAttacks(const bool isBishop)
{
	auto* attacks = (isBishop ? g_arrBishopAttack : g_arrRookAttack);
	auto* attackIndex = (isBishop ? g_arrBishopAttackIndex : g_arrRookAttackIndex);
	auto* blockMask = (isBishop ? g_arrBishopBlockMask : g_arrRookBlockMask);
	auto* shift = (isBishop ? g_arrBishopShiftBits : g_arrRookShiftBits);
#if defined HAVE_BMI2
#else
	auto* magic = (isBishop ? g_arrBishopMagic : g_arrRookMagic);
#endif
	int index = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		blockMask[sq] = (isBishop ? bishopBlockMaskCalc(sq) : rookBlockMaskCalc(sq));
		attackIndex[sq] = index;

		const int num1s = (isBishop ? g_arrBishopBlockBits[sq] : g_arrRookBlockBits[sq]);
		for (int i = 0; i < (1 << num1s); ++i)
		{
			const Bitboard occupied = this->indexToOccupied(i, num1s, blockMask[sq]);
#if defined HAVE_BMI2
			attacks[index + (occupied & blockMask[sq]).occupiedToIndex( blockMask[sq])] = attackCalc(sq, occupied, isBishop);
#else
			attacks[index + occupied.occupiedToIndex( magic[sq], shift[sq])] =
				this->attackCalc(sq, occupied, isBishop);
#endif
		}
		index += 1 << (64 - shift[sq]);
	}
}

// LanceBlockMask, g_arrLanceAttack の値を設定する。
void Initializer::initLanceAttacks() {
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			const Bitboard blockMask = lanceBlockMask(sq);
			//const int num1s = blockMask.popCount(); // 常に 7
			const int num1s = 7;
			assert(num1s == blockMask.popCount());
			for (int i = 0; i < (1 << num1s); ++i) {
				Bitboard occupied = indexToOccupied(i, num1s, blockMask);
				g_arrLanceAttack[c][sq][i] = lanceAttackCalc(c, sq, occupied);
			}
		}
	}
}

void Initializer::initKingAttacks() {
	for (Square sq = I9; sq < SquareNum; ++sq)
		g_arrKingAttack[sq] = Bitboard::allOneBB().rookAttack(sq) | Bitboard::allOneBB().bishopAttack(sq);
}

void Initializer::initGoldAttacks() {
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_arrGoldAttack[c][sq] = (Bitboard::kingAttack(sq) & inFrontMask(c, Util_Square::MakeRank(sq))) | Bitboard::allOneBB().rookAttack(sq);
}

void Initializer::initSilverAttacks() {
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_arrSilverAttack[c][sq] = (Bitboard::kingAttack(sq) & inFrontMask(c, Util_Square::MakeRank(sq))) | Bitboard::allOneBB().bishopAttack(sq);
}

void Initializer::initKnightAttacks() {
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_arrKnightAttack[c][sq] = Bitboard::allZeroBB();
			const Bitboard bb = pawnAttack(c, sq);
			if (bb.isNot0())
				g_arrKnightAttack[c][sq] = bishopStepAttacks(bb.constFirstOneFromI9()) & inFrontMask(c, Util_Square::MakeRank(sq));
		}
	}
}

void Initializer::initPawnAttacks() {
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_arrPawnAttack[c][sq] = silverAttack(c, sq) ^ Bitboard::allOneBB().bishopAttack(sq);
}

void Initializer::initSquareRelation() {
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		const File file1 = Util_Square::MakeFile(sq1);
		const Rank rank1 = Util_Square::MakeRank(sq1);
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			const File file2 = Util_Square::MakeFile(sq2);
			const Rank rank2 = Util_Square::MakeRank(sq2);
			SquareRelation[sq1][sq2] = DirecMisc;
			if (sq1 == sq2) continue;

			if (file1 == file2)
				SquareRelation[sq1][sq2] = DirecFile;
			else if (rank1 == rank2)
				SquareRelation[sq1][sq2] = DirecRank;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file1 - file2))
				SquareRelation[sq1][sq2] = DirecDiagNESW;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file2 - file1))
				SquareRelation[sq1][sq2] = DirecDiagNWSE;
		}
	}
}

// 障害物が無いときの利きの Bitboard
// g_arrRookAttack, g_arrBishopAttack, g_arrLanceAttack を設定してから、この関数を呼ぶこと。
void Initializer::initAttackToEdge() {
	for (Square sq = I9; sq < SquareNum; ++sq) {
		g_arrRookAttackToEdge[sq] = Bitboard::allZeroBB().rookAttack(sq);
		g_arrBishopAttackToEdge[sq] = Bitboard::allZeroBB().bishopAttack(sq);
		g_arrLanceAttackToEdge[Black][sq] = Bitboard::allZeroBB().lanceAttack(Black, sq);
		g_arrLanceAttackToEdge[White][sq] = Bitboard::allZeroBB().lanceAttack(White, sq);
	}
}

void Initializer::initBetweenBB() {
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			g_arrBetweenBB[sq1][sq2] = Bitboard::allZeroBB();
			if (sq1 == sq2) continue;
			const Direction direc = squareRelation(sq1, sq2);
			if (direc & DirecCross)
				g_arrBetweenBB[sq1][sq2] = Bitboard::setMaskBB(sq2).rookAttack(sq1) & Bitboard::setMaskBB(sq1).rookAttack(sq2);
			else if (direc & DirecDiag)
				g_arrBetweenBB[sq1][sq2] = Bitboard::setMaskBB(sq2).bishopAttack(sq1) & Bitboard::setMaskBB(sq1).bishopAttack(sq2);
		}
	}
}

void Initializer::initCheckTable() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = oppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_arrGoldCheckTable[c][sq] = Bitboard::allZeroBB();
			Bitboard checkBB = goldAttack(opp, sq);
			while (checkBB.isNot0()) {
				const Square checkSq = checkBB.firstOneFromI9();
				g_arrGoldCheckTable[c][sq] |= goldAttack(opp, checkSq);
			}
			g_arrGoldCheckTable[c][sq].andEqualNot(Bitboard::setMaskBB(sq) | goldAttack(opp, sq));
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = oppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_arrSilverCheckTable[c][sq] = Bitboard::allZeroBB();

			Bitboard checkBB = silverAttack(opp, sq);
			while (checkBB.isNot0()) {
				const Square checkSq = checkBB.firstOneFromI9();
				g_arrSilverCheckTable[c][sq] |= silverAttack(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? inFrontMask<Black, Rank6>() : inFrontMask<White, Rank4>());
			checkBB = goldAttack(opp, sq);
			while (checkBB.isNot0()) {
				const Square checkSq = checkBB.firstOneFromI9();
				// 移動元が敵陣である位置なら、金に成って王手出来る。
				g_arrSilverCheckTable[c][sq] |= (silverAttack(opp, checkSq) & TRank789BB);
			}

			const Bitboard TRank6BB = (c == Black ? rankMask<Rank6>() : rankMask<Rank4>());
			// 移動先が3段目で、4段目に移動したときも、成ることが出来る。
			checkBB = goldAttack(opp, sq) & TRank789BB;
			while (checkBB.isNot0()) {
				const Square checkSq = checkBB.firstOneFromI9();
				g_arrSilverCheckTable[c][sq] |= (silverAttack(opp, checkSq) & TRank6BB);
			}
			g_arrSilverCheckTable[c][sq].andEqualNot(Bitboard::setMaskBB(sq) | silverAttack(opp, sq));
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = oppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_arrKnightCheckTable[c][sq] = Bitboard::allZeroBB();

			Bitboard checkBB = knightAttack(opp, sq);
			while (checkBB.isNot0()) {
				const Square checkSq = checkBB.firstOneFromI9();
				g_arrKnightCheckTable[c][sq] |= knightAttack(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? inFrontMask<Black, Rank6>() : inFrontMask<White, Rank4>());
			checkBB = goldAttack(opp, sq) & TRank789BB;
			while (checkBB.isNot0()) {
				const Square checkSq = checkBB.firstOneFromI9();
				g_arrKnightCheckTable[c][sq] |= knightAttack(opp, checkSq);
			}
		}
	}

	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = oppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_arrLanceCheckTable[c][sq] = lanceAttackToEdge(opp, sq);

			const Bitboard TRank789BB = (c == Black ? inFrontMask<Black, Rank6>() : inFrontMask<White, Rank4>());
			Bitboard checkBB = goldAttack(opp, sq) & TRank789BB;
			while (checkBB.isNot0()) {
				const Square checkSq = checkBB.firstOneFromI9();
				g_arrLanceCheckTable[c][sq] |= lanceAttackToEdge(opp, checkSq);
			}
			g_arrLanceCheckTable[c][sq].andEqualNot(Bitboard::setMaskBB(sq) | pawnAttack(opp, sq));
		}
	}
}

void Initializer::initSquareDistance() {
	for (Square sq0 = I9; sq0 < SquareNum; ++sq0) {
		for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
			switch (squareRelation(sq0, sq1)) {
			case DirecMisc:
				// DirecMisc な関係は全て距離 1 にしてもKPE学習には問題無いんだけれど。
				SquareDistance[sq0][sq1] = 0;
				if (knightAttack(Black, sq0).isSet(sq1) || knightAttack(White, sq0).isSet(sq1))
					SquareDistance[sq0][sq1] = 1;
				break;
			case DirecFile:
				SquareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaN));
				break;
			case DirecRank:
				SquareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaE));
				break;
			case DirecDiagNESW:
				SquareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaNE));
				break;
			case DirecDiagNWSE:
				SquareDistance[sq0][sq1] = abs(static_cast<int>(sq0 - sq1) / static_cast<int>(DeltaNW));
				break;
			default: UNREACHABLE;
			}
		}
	}
}

void Initializer::initTable() {

#ifndef SKIP_LONG_TIME_EVAL
	SYNCCOUT << "(^q^)I1: SKIP! (long time)initAttacks!" << SYNCENDL;
	this->initAttacks(false);
#endif

	SYNCCOUT << "(^q^)I2: initAttacks!" << SYNCENDL;
	this->initAttacks(true);

	SYNCCOUT << "(^q^)I3: initKingAttacks!" << SYNCENDL;
	this->initKingAttacks();

	SYNCCOUT << "(^q^)I4: initGoldAttacks!" << SYNCENDL;
	this->initGoldAttacks();

	SYNCCOUT << "(^q^)I5: initSilverAttacks!" << SYNCENDL;
	this->initSilverAttacks();

	SYNCCOUT << "(^q^)I6: initPawnAttacks!" << SYNCENDL;
	this->initPawnAttacks();

	SYNCCOUT << "(^q^)I7: initKnightAttacks!" << SYNCENDL;
	this->initKnightAttacks();

	SYNCCOUT << "(^q^)I8: initLanceAttacks!" << SYNCENDL;
	this->initLanceAttacks();

	SYNCCOUT << "(^q^)I9: initSquareRelation!" << SYNCENDL;
	this->initSquareRelation();

	SYNCCOUT << "(^q^)I10: initAttackToEdge!" << SYNCENDL;
	this->initAttackToEdge();

	SYNCCOUT << "(^q^)I11: initBetweenBB!" << SYNCENDL;
	this->initBetweenBB();

	SYNCCOUT << "(^q^)I12: initCheckTable!" << SYNCENDL;
	this->initCheckTable();

	SYNCCOUT << "(^q^)I13: initSquareDistance!" << SYNCENDL;
	this->initSquareDistance();

	SYNCCOUT << "(^q^)I14: Book::init!" << SYNCENDL;
	Book::init();

	SYNCCOUT << "(^q^)I15: initSearchTable!" << SYNCENDL;
	initSearchTable();
}

#if defined FIND_MAGIC
// square の位置の rook, bishop それぞれのMagic Bitboard に使用するマジックナンバーを見つける。
// isBishop  : true なら bishop, false なら rook のマジックナンバーを見つける。
u64 Initializer::findMagic(const Square square, const bool isBishop) {
	Bitboard occupied[1<<14];
	Bitboard attack[1<<14];
	Bitboard attackUsed[1<<14];
	Bitboard mask = (isBishop ? bishopBlockMaskCalc(square) : rookBlockMaskCalc(square));
	int num1s = (isBishop ? g_arrBishopBlockBits[square] : g_arrRookBlockBits[square]);

	// n bit の全ての数字 (利きのあるマスの全ての 0 or 1 の組み合わせ)
	for (int i = 0; i < (1 << num1s); ++i) {
		occupied[i] = indexToOccupied(i, num1s, mask);
		attack[i] = attackCalc(square, occupied[i], isBishop);
	}

	for (u64 k = 0; k < UINT64_C(100000000); ++k) {
		const u64 magic = g_mt64bit.randomFewBits();
		bool fail = false;

		// これは無くても良いけど、少しマジックナンバーが見つかるのが早くなるはず。
		if (count1s((mask.merge() * magic) & UINT64_C(0xfff0000000000000)) < 6)
			continue;

		std::fill(std::begin(attackUsed), std::end(attackUsed), allZeroBB());

		for (int i = 0; !fail && i < (1 << num1s); ++i) {
			const int shiftBits = (isBishop ? g_arrBishopShiftBits[square] : g_arrRookShiftBits[square]);
			const u64 index = occupiedToIndex(occupied[i], magic, shiftBits);
			if      (attackUsed[index] == allZeroBB())
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
