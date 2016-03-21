#pragma once

#include "../../stdafx.h"
#include "../../n240_150_move.hpp"
#include "../n200_score___/n200_100_score.hpp"

class MoveScore {
public:
	Move move;
	enum Score score;
public:
	MoveScore(Move move, enum Score score);
	
	Move GetMove(void);
};