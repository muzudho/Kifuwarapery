#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_190_pawnAttackBb.hpp"
#include "n170_070_ptAbstract.hpp"

class PtPawn : PtAbstract {
public:

	inline PieceType GetNumber() const {
		return PieceType::N01_Pawn;
	}

	inline Bitboard GetAttacksFrom(const Bitboard& occupied, const Color c, const Square sq) const {
		return g_pawnAttackBb.GetControllBb(c, sq);;
	}

};


extern PtPawn g_ptPawn;
