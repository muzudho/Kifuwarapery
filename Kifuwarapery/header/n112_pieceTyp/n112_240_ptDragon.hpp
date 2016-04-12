#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtDragon : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N14_Dragon;
	}

};


extern PtDragon g_ptDragon;
