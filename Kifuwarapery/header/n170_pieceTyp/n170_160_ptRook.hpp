#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n170_070_ptAbstract.hpp"

class PtRook : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N06_Rook;
	}

};


extern PtRook g_ptRook;