#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_200_handPiece.hpp"


class UtilHandPiece {
public:
	static inline HandPiece FromPieceType(const PieceType pt) { return g_pieceTypeToHandPieceTable[pt]; }
};


