#pragma once

#include "../../header/n200_score___/n200_100_score.hpp"
#include "../../header/n240_position/n240_150_move.hpp"

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