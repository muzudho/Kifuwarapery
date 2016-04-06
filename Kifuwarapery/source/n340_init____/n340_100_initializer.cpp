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


void Initializer::InitSquareDistance() {
	for (Square sq0 = I9; sq0 < SquareNum; ++sq0) {
		for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
			switch (SquareRelation::GetSquareRelation(sq0, sq1)) {
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
	// 障害物が無いときの利きの Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack を設定してから、この関数を呼ぶこと。
	g_rookAttackBb.InitializeToEdge();
	g_bishopAttackBb.InitializeToEdge();
	g_lanceAttackBb.InitializeToEdge();

	SYNCCOUT << "(^q^)I11: initBetweenBB!" << SYNCENDL;
	g_betweenBb.Initialize();

	SYNCCOUT << "(^q^)I12: initCheckTable!" << SYNCENDL;
    g_goldAttackBb.InitCheckTableGold();
	g_silverAttackBb.InitCheckTableSilver();
	g_knightAttackBb.InitCheckTableKnight();
	g_lanceAttackBb.InitCheckTableLance();

	SYNCCOUT << "(^q^)I13: initSquareDistance!" << SYNCENDL;
	this->InitSquareDistance();

	SYNCCOUT << "(^q^)I14: Book::init!" << SYNCENDL;
	Book::Init();

	SYNCCOUT << "(^q^)I15: initSearchTable!" << SYNCENDL;
	InitSearchTable();
}


