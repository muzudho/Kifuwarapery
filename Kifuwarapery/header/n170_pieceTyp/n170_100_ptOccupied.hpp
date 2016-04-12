#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n170_070_ptAbstract.hpp"

class PtOccupied : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N00_Occupied;
	}

};


extern PtOccupied g_ptOccupied;
