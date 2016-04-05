#pragma once

#include "../../header/n119_score___/n119_100_utilScore.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"

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