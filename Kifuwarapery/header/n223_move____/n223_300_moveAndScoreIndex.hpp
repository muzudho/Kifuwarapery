#pragma once

#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n165_movStack/n165_400_move.hpp"


// 指し手と、評価値。
class MoveAndScoreIndex {
public:

	// 指し手
	const Move m_move;

	// 評価値
	const enum ScoreIndex m_scoreIndex;

public:

	MoveAndScoreIndex(Move move, enum ScoreIndex scoreIndex);
	
	// 指し手
	Move GetMove(void);

};