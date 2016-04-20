#pragma once

#include "../n119_score___/n119_090_score.hpp"
#include "../n165_movStack/n165_400_move.hpp"


// 指し手と、評価値。
class MoveAndScore {
public:

	// 指し手
	const Move m_move;

	// 評価値
	const enum Score m_score;

public:

	MoveAndScore(Move move, enum Score score);
	
	// 指し手
	Move GetMove(void);

};