#pragma once

#include "n160_100_bitboard.hpp"

extern KingAttackBb g_kingAttackBb;
extern RookAttackBb g_rookAttackBb;


class DragonAttackBb {
public:
	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Square sq) const
	{
		return g_rookAttackBb.GetControllBb(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}

	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return g_rookAttackBb.GetControllBbToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}
};
