#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n112_pieceTyp/n112_070_ptAbstract.hpp"

class PtGold : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N07_Gold;
	}

};


extern PtGold g_ptGold;
