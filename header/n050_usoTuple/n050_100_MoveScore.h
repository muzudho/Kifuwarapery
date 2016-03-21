#pragma once

#include "../../stdafx.h"
#include "../../move.hpp"
#include "../../score.hpp"

class MoveScore {
public:
	Move move;
	enum Score score;
public:
	MoveScore(Move move, enum Score score);
	
	Move GetMove(void);
};