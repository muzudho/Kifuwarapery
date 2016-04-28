#pragma once


#include <string>
#include <sstream>
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_130_lanceAttackBb.hpp"
#include "../n160_board___/n160_150_rookAttackBb.hpp"
#include "../n160_board___/n160_220_queenAttackBb.hpp"
#include "../n160_board___/n160_230_setMaskBb.hpp"
#include "../n223_move____/n223_060_stats.hpp"
#include "../n350_pieceTyp/n350_040_ptEvent.hpp"
#include "../n350_pieceTyp/n350_500_ptPrograms.hpp"
#include "../n560_timeMng_/n560_500_timeManager.hpp"
#include "../n640_searcher/n640_128_signalsType.hpp"
#include "../n640_searcher/n640_450_rootMove.hpp"
#include "../n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n760_thread__/n760_400_herosPub.hpp"
#include "../n800_learn___/n800_100_stopwatch.hpp"
//#include "../n885_searcher/n885_600_iterativeDeepeningLoop.hpp"
#include "n885_040_rucksack.hpp"	// FIXME:

using namespace std;

using History = Stats<false>;
using Gains = Stats<true>;


class Hitchhiker {
public:

	// 考える？
	// メイン・スレッドの　アイドル・ループの中で呼び出される。
	// （１）入玉勝ち判定
	// （２）反復深化探索を行うぜ☆
	static void	Think(
		Rucksack& captainsRucksack // キャプテンの持っているＳｅａｒｃｈｅｒ☆
	);

	// 入玉勝ち判定
	static bool Hitchhiker::IsNyugyokuWin(const Position& pos);

};