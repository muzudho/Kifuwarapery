#pragma once

#include "n112_050_pieceType.hpp"
#include "n112_070_ptAbstract.hpp"

class PtProSilver : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N12_ProSilver;
	}

};


extern PtProSilver g_ptProSilver;
