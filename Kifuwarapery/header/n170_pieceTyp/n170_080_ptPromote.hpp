#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n170_070_ptAbstract.hpp"

class PtPromote : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::PTPromote;
	}

};


extern PtPromote g_ptPromote;
