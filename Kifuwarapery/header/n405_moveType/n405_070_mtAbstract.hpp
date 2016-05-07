#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_020_moveStack.hpp"


class MoveTypeAbstract {
public:

	virtual MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all=false) const = 0;

};

