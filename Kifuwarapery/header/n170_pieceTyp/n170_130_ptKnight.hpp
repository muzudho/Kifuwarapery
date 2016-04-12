#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_180_knightAttackBb.hpp"
#include "n170_070_ptAbstract.hpp"

class PtKnight : public PtAbstract {
public:

	inline PieceType GetNumber() const {
		return PieceType::N03_Knight;
	}

	inline Bitboard GetAttacks2From(const Bitboard& occupied, const Color c, const Square sq) const {
		return g_knightAttackBb.GetControllBb(c, sq);
	}

};


extern PtKnight g_ptKnight;
