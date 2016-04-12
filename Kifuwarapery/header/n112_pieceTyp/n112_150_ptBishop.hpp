#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtBishop : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N05_Bishop;
	}

};


extern PtBishop g_ptBishop;
