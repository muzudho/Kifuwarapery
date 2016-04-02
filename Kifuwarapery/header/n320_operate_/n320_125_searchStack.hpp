#pragma once


class Position;
struct SplitPoint;


class SearchStack {
public:
	SplitPoint* splitPoint;
	Ply ply;
	Move currentMove;
	Move excludedMove; // todo: これは必要？
	Move killers[2];
	Depth reduction;
	Score staticEval;
	bool skipNullMove;
	EvalSum staticEvalRaw; // 評価関数の差分計算用、値が入っていないときは [0] を ScoreNotEvaluated にしておく。
						   // 常に Black 側から見た評価値を入れておく。
						   // 0: 双玉に対する評価値, 1: 先手玉に対する評価値, 2: 後手玉に対する評価値
};
