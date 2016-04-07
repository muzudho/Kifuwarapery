#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_250_squareRelation.hpp"
#include "../../header/n160_board___/n160_600_bitboardAll.hpp"
#include "../../header/n260_evaluate/n260_700_evaluate.hpp"
#include "../../header/n280_move____/n280_200_mt64bit.hpp"
#include "../../header/n300_book____/n300_100_book.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n340_init____/n340_100_initializer.hpp"


extern SquareRelation g_squareRelation;



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
	g_squareRelation.Initialize();

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
	g_squareDistance.InitSquareDistance();

	SYNCCOUT << "(^q^)I14: Book::init!" << SYNCENDL;
	Book::Init();

	SYNCCOUT << "(^q^)I15: initSearchTable!" << SYNCENDL;
	InitSearchTable();
}


