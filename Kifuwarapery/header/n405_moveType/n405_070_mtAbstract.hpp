#pragma once


#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"


class MoveTypeAbstract {
public:

	virtual MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all=false) const = 0;

};

