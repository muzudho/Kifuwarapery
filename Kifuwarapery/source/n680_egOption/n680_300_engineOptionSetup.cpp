#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n520_evaluate/n520_500_kkKkpKppStorage1.hpp"
#include "..\..\header\n680_egOption/n680_245_engineOption.hpp"
#include "..\..\header\n680_egOption\n680_300_engineOptionSetup.hpp"
#include "../../header/n885_searcher/n885_040_rucksack.hpp"


// 初期化の値を取ってくるのに使います。
namespace {
	void onHashSize(Rucksack* s, const EngineOptionable& opt) { s->m_tt.SetSize(opt); }
	void onClearHash(Rucksack* s, const EngineOptionable&) { s->m_tt.Clear(); }
	void onEvalDir(Rucksack*, const EngineOptionable& opt) {
		std::unique_ptr<KkKkpKppStorage1>(new KkKkpKppStorage1)->Init(opt, true);
	}
	void onThreads(Rucksack* s, const EngineOptionable&) { s->m_ownerHerosPub.ReadUSIOptions(s); }
	// 論理的なコア数の取得
	inline int cpuCoreCount() {
		// todo: boost::thread::physical_concurrency() を使うこと。
		// std::thread::hardware_concurrency() は 0 を返す可能性がある。
		return std::max(static_cast<int>(std::thread::hardware_concurrency() / 2), 1);
	}
}


// USIエンジン用のオプションを初期設定するぜ☆
void EngineOptionSetup::Initialize(EngineOptionsMap* pMap, Rucksack * pRucksack)
{
	pMap->Put("USI_Hash"					, EngineOption(256, 1, 65536, onHashSize, pRucksack));
	pMap->Put("Clear_Hash"					, EngineOption(onClearHash, pRucksack));
	pMap->Put("Book_File"					, EngineOption("book/20150503/book.bin"));
	pMap->Put("Best_Book_Move"				, EngineOption(false));
	pMap->Put("OwnBook"						, EngineOption(true));
	pMap->Put("Min_Book_Ply"				, EngineOption(SHRT_MAX, 0, SHRT_MAX));
	pMap->Put("Max_Book_Ply"				, EngineOption(SHRT_MAX, 0, SHRT_MAX));
	pMap->Put("Min_Book_Score"				, EngineOption(-180, -ScoreInfinite, ScoreInfinite));
	pMap->Put("Eval_Dir"					, EngineOption("20151105", onEvalDir));
	pMap->Put("Write_Synthesized_Eval"		, EngineOption(false));
	pMap->Put("USI_Ponder"					, EngineOption(true));
	pMap->Put("Byoyomi_Margin"				, EngineOption(500, 0, INT_MAX));//元の値：0.5秒☆？（＾ｑ＾）
	pMap->Put("MultiPV"						, EngineOption(1, 1, Move::m_MAX_LEGAL_MOVES));
	pMap->Put("Skill_Level"					, EngineOption(20, 0, 20));
	pMap->Put("Max_Random_Score_Diff"		, EngineOption(0, 0, ScoreMate0Ply));
	pMap->Put("Max_Random_Score_Diff_Ply"	, EngineOption(40, 0, SHRT_MAX));

	//────────────────────────────────────────────────────────────────────────────────
	// 時間制御関連☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	pMap->Put("Emergency_Move_Horizon"		, EngineOption(40, 0, 50));
	//pMap->Put("Emergency_Base_Time"			, EngineOption(   200,  0, 30000));	// 緊急時用に残しておこうというタイム（ミリ秒）か☆？
	//pMap->Put("Emergency_Move_Time"			, EngineOption(    70,  0,  5000));	// 緊急時用に残しておこうというタイム（ミリ秒）か☆？
	//pMap->Put("Slow_Mover"					, EngineOption(   100, 10,  1000));//スロー・ムーバーとは何なのか☆？（＾ｑ＾）？
#if defined(FISCHER_RULE)
	pMap->Put("Minimum_Thinking_Time"		, EngineOption(  9000,  0, INT_MAX));// 10秒加算されると見越して、9秒は最低使えるということにしようぜ☆（＾ｑ＾）
#else
	pMap->Put("Minimum_Thinking_Time", EngineOption(1500, 0, INT_MAX));// 元の値：初期値 1500（1.5）秒☆
#endif

	//────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────
	pMap->Put("Max_Threads_per_Split_Point"	, EngineOption(		5,  4,     8, onThreads, pRucksack));
	pMap->Put("Threads"						, EngineOption(cpuCoreCount(), 1, g_MaxThreads, onThreads, pRucksack));
	pMap->Put("Use_Sleeping_Threads"		, EngineOption(false));
#if defined BISHOP_IN_DANGER
	(*this)["Danger_Demerit_Score"] = EngineOption(700, SHRT_MIN, SHRT_MAX);
#endif
}
