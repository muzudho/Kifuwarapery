#pragma once

#include "../../header/n200_score___/n200_100_utilScore.hpp"
#include "../../header/n240_position/n240_150_move.hpp"

// �w����ƁA�]���l�B
class MoveScore {
public:
	// �w����
	Move move;
	// �]���l
	enum Score score;
public:
	MoveScore(Move move, enum Score score);
	
	// �w����
	Move GetMove(void);
};