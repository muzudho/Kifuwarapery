#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtHorse : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N13_Horse;
	}

};


extern PtHorse g_ptHorse;
