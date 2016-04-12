#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtPieceTypeNum : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N15_PieceTypeNum;
	}

};


extern PtPieceTypeNum g_ptPieceTypeNum;
