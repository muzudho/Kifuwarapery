#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtKing : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N08_King;
	}

};


extern PtKing g_ptKing;
