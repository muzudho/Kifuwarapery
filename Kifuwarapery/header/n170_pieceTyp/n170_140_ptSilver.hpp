#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n170_070_ptAbstract.hpp"

class PtSilver : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N04_Silver;
	}

};


extern PtSilver g_ptSilver;