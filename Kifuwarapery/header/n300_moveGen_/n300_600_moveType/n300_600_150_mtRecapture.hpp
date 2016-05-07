#pragma once


#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../n300_moveGen_/n300_400_genMove_/n300_400_350_PieceMovesGenerator.hpp"
#include "n300_600_070_mtAbstract.hpp"


class MoveTypeRecapture : public MoveTypeAbstract {
public:
	MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all = false
		) const override {
		return moveStackList;
	}

};


