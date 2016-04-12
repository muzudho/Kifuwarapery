#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n110_square__/n110_400_squareRelation.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n160_board___/n160_110_silverAttackBb.hpp"
#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"
#include "../../header/n160_board___/n160_130_lanceAttackBb.hpp"
#include "../../header/n160_board___/n160_140_goldAttackBb.hpp"
#include "../../header/n160_board___/n160_150_rookAttackBb.hpp"
#include "../../header/n160_board___/n160_160_kingAttackBb.hpp"
#include "../../header/n160_board___/n160_180_knightAttackBb.hpp"
#include "../../header/n160_board___/n160_190_pawnAttackBb.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n240_tt______/n240_300_tt.hpp"
#include "../../header/n260_evaluate/n260_500_kkKkpKppStorage1.hpp"
#include "../../header/n300_book____/n300_500_book.hpp"
#include "../../header/n450_thread__/n450_400_threadPool.hpp"
#include "../../header/n900_main____/n900_200_searcher.hpp"
#include "..\..\header\n900_main____/n900_300_usiLoop.hpp"
#include "..\..\header\n900_main____\n900_400_main01.hpp"




Main01::Main01()
{
	this->searcher = std::unique_ptr<Searcher>(new Searcher);
}

Main01::~Main01()
{
	this->searcher.reset();
}

void Main01::Initialize()
{
	//────────────────────────────────────────────────────────────────────────────────
	// ビットボードとテーブルの初期化☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	{
		SYNCCOUT << "(^q^)main(1/6): (long time)initTable!" << SYNCENDL;
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

	SYNCCOUT << "(^q^)main(2/6): initZobrist!" << SYNCENDL;
	Position::InitZobrist();

	SYNCCOUT << "(^q^)main(3/6): searcher->init!" << SYNCENDL;
	this->searcher->Init();
	// 一時オブジェクトの生成と破棄

	SYNCCOUT << "(^q^)main(4/6): start Evaluater init!" << SYNCENDL;
	std::unique_ptr<KkKkpKppStorage1>(new KkKkpKppStorage1)->Init(this->searcher->m_engineOptions["Eval_Dir"], true);
	SYNCCOUT << "(^q^)main(5/6): end Evaluater init! ----> doUSICommandLoop" << SYNCENDL;

}

void Main01::Body(int argc, char* argv[])
{
	UsiLoop usiLoop;
	usiLoop.Mainloop(argc, argv, *this->searcher);
}

void Main01::Finalize()
{
	SYNCCOUT << "(^q^)main(6/6): threads.exit! ----> doUSICommandLoop" << SYNCENDL;
	this->searcher->m_threads.Exit();
}
