#pragma once

#include "n160_100_bitboard.hpp"

extern KingAttackBb g_kingAttackBb;
extern RookAttackBb g_rookAttackBb;


class DragonAttackBb {
public:
	inline Bitboard DragonAttack(const Bitboard* thisBitboard, const Square sq) const
	{
		return g_rookAttackBb.RookAttack(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}

	inline Bitboard DragonAttackToEdge(const Square sq) {
		return g_rookAttackBb.RookAttackToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}
};
