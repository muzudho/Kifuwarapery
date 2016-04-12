#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n170_070_ptAbstract.hpp"

class PtBishop : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N05_Bishop;
	}

};


extern PtBishop g_ptBishop;
