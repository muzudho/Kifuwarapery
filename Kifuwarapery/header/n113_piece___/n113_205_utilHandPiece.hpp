#pragma once

#include <iostream>
#include <string>
#include <cassert>
#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n113_piece___/n113_200_handPiece.hpp"


class UtilHandPiece {
public:
	static inline HandPiece FromPieceType(const PieceType pt) { return g_pieceTypeToHandPieceTable[pt]; }
};


