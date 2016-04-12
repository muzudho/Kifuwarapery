#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n112_pieceTyp/n112_070_ptAbstract.hpp"

class PtGoldHorseDragon : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N16_GoldHorseDragon;
	}

};


extern PtGoldHorseDragon g_ptGoldHorseDragon;
