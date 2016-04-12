#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "n170_070_ptAbstract.hpp"

class PtProSilver : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N12_ProSilver;
	}

};


extern PtProSilver g_ptProSilver;
