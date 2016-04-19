#pragma once


#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "n405_040_mtEvent.hpp"


class MoveTypeAbstract {
public:

	virtual MoveStack* GenerateMoves1(
		MoveTypeEvent& mtEvent,
		bool all=false
	) const = 0;

};

