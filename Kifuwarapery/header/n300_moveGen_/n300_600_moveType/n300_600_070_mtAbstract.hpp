#pragma once


#include "../../n220_position/n220_650_position.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"


class MoveTypeAbstract {
public:

	virtual MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all=false) const = 0;

};

