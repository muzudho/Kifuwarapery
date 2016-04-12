#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtLance : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N02_Lance;
	}

};


extern PtLance g_ptLance;
