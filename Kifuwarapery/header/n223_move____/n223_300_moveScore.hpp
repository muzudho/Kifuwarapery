#pragma once

#include "../../header/n119_score___/n119_100_utilScore.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"

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