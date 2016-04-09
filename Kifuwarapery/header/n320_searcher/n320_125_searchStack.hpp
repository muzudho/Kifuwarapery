#pragma once

#include "../n220_position/n220_600_position.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n260_evaluate/n260_600_EvalSum.hpp"


struct SplitPoint;


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
