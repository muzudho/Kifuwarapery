#pragma once

#include "header/n200_score___/n200_100_score.hpp"
#include "header/n240_position/n240_150_move.hpp"

class MoveScore {
public:
	Move move;
	enum Score score;
public:
	MoveScore(Move move, enum Score score);
	
	Move GetMove(void);
};