#pragma once

#include "../n119_score___/n119_090_score.hpp"	//Ply,Score
#include "../n119_score___/n119_500_EvalSum.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n223_move____/n223_200_depth.hpp"

struct SplitPoint;// SearchStack,NextmoveEvent(ＭｏｖｅＰｉｃｋｅｒ),SplitPoint が三つ巴になっている☆（＾ｑ＾）


class SearchStack {
public:

	SplitPoint*		m_splitPoint;

	Ply				m_ply;

	Move			m_currentMove;

	Move			m_excludedMove; // todo: これは必要？

	Move			m_killers[2];

	Depth			m_reduction;

	Score			m_staticEval;

	bool			m_skipNullMove;

	EvalSum			m_staticEvalRaw;	// 評価関数の差分計算用、値が入っていないときは [0] を ScoreNotEvaluated にしておく。
									// 常に Black 側から見た評価値を入れておく。
									// 0: 双玉に対する評価値, 1: 先手玉に対する評価値, 2: 後手玉に対する評価値
};
