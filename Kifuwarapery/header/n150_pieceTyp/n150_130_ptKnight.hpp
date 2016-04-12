#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n112_pieceTyp/n112_070_ptAbstract.hpp"

class PtKnight : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N03_Knight;
	}

};


extern PtKnight g_ptKnight;
