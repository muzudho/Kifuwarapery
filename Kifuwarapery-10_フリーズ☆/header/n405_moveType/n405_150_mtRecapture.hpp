#pragma once


#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "n405_070_mtAbstract.hpp"


class MoveTypeRecapture : public MoveTypeAbstract {
public:
	MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all = false) const {
		return moveStackList;
	}

};


