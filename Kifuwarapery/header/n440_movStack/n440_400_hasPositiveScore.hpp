#pragma once


#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"


struct HasPositiveScore {
	bool operator () (const MoveStack& ms) {
		return 0 < ms.m_score;
	}
};
