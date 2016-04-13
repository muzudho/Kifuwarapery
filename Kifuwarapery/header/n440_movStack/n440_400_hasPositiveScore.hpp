#pragma once


#include "../n165_movStack/n165_500_moveStack.hpp"


struct HasPositiveScore {
	bool operator () (const MoveStack& ms) {
		return 0 < ms.m_score;
	}
};
