#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"

class PtAbstract {
public:

	virtual inline PieceType GetNumber() const = 0;

	virtual inline Bitboard GetAttacks2From(const Bitboard& occupied, const Color c, const Square sq) const = 0;

};
