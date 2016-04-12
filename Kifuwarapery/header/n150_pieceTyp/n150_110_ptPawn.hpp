#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n112_pieceTyp/n112_070_ptAbstract.hpp"

class PtPawn : PtAbstract {
public:

	PieceType inline GetNumber() const {
		return PieceType::N01_Pawn;
	}

};


extern PtPawn g_ptPawn;
