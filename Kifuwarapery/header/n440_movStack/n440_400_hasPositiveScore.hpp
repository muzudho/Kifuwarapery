#pragma once


#include "../n350_pieceTyp/n350_020_moveStack.hpp"


struct HasPositiveScore {
	bool operator () (const MoveStack& ms) {
		return 0 < ms.m_score;
	}
};
