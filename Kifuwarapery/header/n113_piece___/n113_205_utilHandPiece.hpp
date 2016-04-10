#pragma once

#include "../n113_piece___/n113_100_pieceType.hpp"
#include "../n113_piece___/n113_200_handPiece.hpp"


class UtilHandPiece {
public:
	static inline HandPiece FromPieceType(const PieceType pt) { return g_pieceTypeToHandPieceTable[pt]; }
};


