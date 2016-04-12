#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtPromote : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::PTPromote;
	}

};


extern PtPromote g_ptPromote;
