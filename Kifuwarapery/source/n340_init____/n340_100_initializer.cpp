#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_600_bitboardAll.hpp"
#include "../../header/n260_evaluate/n260_700_evaluate.hpp"
#include "../../header/n280_move____/n280_200_mt64bit.hpp"
#include "../../header/n300_book____/n300_100_book.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n340_init____/n340_100_initializer.hpp"


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
	g_rookAttackBb.InitializeToEdge();

	g_bishopAttackBb.InitializeToEdge();

	g_lanceAttackBb.InitializeToEdge();
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
	SYNCCOUT << "(^q^)I1: SKIP! (long time)InitRookAttacks!" << SYNCENDL;
	g_rookAttackBb.InitRookAttacks();
#endif

	SYNCCOUT << "(^q^)I2: initAttacks!" << SYNCENDL;
	g_bishopAttackBb.InitBishopAttacks();

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


