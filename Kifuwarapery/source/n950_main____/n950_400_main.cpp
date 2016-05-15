#include <iostream>
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
#include "../../header/n161_sqDistan/n161_500_squareDistance.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n351_bonaDir_/n351_600_utilSquareDistance.hpp"
#include "../../header/n480_tt______/n480_300_tt.hpp"
#include "../../header/n600_book____/n600_500_book.hpp"
#include "../../header/n760_thread__/n760_400_herosPub.hpp"
#include "../../header/n830_evaluate/n830_600_kkKkpKppStorage1.hpp"
#include "../../header/n885_searcher/n885_040_rucksack.hpp"
#include "..\..\header\n950_main____/n950_300_usiLoop.hpp"
#include "..\..\header\n950_main____\n950_400_main01.hpp"




Main01::Main01()
{
	this->m_rucksack = std::unique_ptr<Rucksack>(new Rucksack);
}

Main01::~Main01()
{
	this->m_rucksack.reset();
}

void Main01::Initialize()
{
	//────────────────────────────────────────────────────────────────────────────────
	// ビットボードとテーブルの初期化☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	{
		SYNCCOUT << "(^q^)main(1/6): initTable!" << SYNCENDL;//長い時間
#ifndef SKIP_LONG_TIME_EVAL
		SYNCCOUT << "(^q^)I1: InitRookAttacks!" << SYNCENDL;//長い時間
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
		UtilSquareDistance::InitSquareDistance(g_squareDistance);

		SYNCCOUT << "(^q^)I14: Book::init!" << SYNCENDL;
		Book::Init();

		SYNCCOUT << "(^q^)I15: initSearchTable!" << SYNCENDL;
		InitSearchTable();
	}

	SYNCCOUT << "(^q^)main(2/6): initZobrist!" << SYNCENDL;
	Position::InitZobrist();

	SYNCCOUT << "(^q^)main(3/6): searcher->init!" << SYNCENDL;
	this->m_rucksack->Init();
	// 一時オブジェクトの生成と破棄

	SYNCCOUT << "(^q^)main(4/6): start Evaluater init!" << SYNCENDL;
	std::unique_ptr<KkKkpKppStorage1>(new KkKkpKppStorage1)->Init(this->m_rucksack->m_engineOptions["Eval_Dir"], true);
	SYNCCOUT << "(^q^)main(5/6): end Evaluater init! ----> doUSICommandLoop" << SYNCENDL;

}

void Main01::Body(int argc, char* argv[])
{
	UsiLoop usiLoop;
	usiLoop.Mainloop(argc, argv, *this->m_rucksack);
}

void Main01::Finalize()
{
	SYNCCOUT << "(^q^)main(6/6): threads.exit! ----> doUSICommandLoop" << SYNCENDL;
	this->m_rucksack->m_ownerHerosPub.Exit();
}
