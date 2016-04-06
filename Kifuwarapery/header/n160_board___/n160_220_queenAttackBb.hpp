#pragma once

#include "n160_100_bitboard.hpp"

extern BishopAttackBb g_bishopAttackBb;
extern RookAttackBb g_rookAttackBb;


class QueenAttackBb {
public:

	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Square sq) const
	{
		return g_rookAttackBb.GetControllBb(thisBitboard, sq) | g_bishopAttackBb.BishopAttack(thisBitboard, sq);
	}

};