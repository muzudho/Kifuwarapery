#pragma once

#include "../n119_score___/n119_090_score.hpp"
#include "../n223_move____/n223_100_move.hpp"


// 指し手と、評価値。
class MoveScore {
public:
	// 指し手
	Move move;
	// 評価値
	enum Score score;
public:
	MoveScore(Move move, enum Score score);
	
	// 指し手
	Move GetMove(void);
};