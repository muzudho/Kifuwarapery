#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n170_070_ptAbstract.hpp"

class PtProLance : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N10_ProLance;
	}

};


extern PtProLance g_ptProLance;
