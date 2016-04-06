#pragma once

#include "n160_100_bitboard.hpp"

extern BishopAttackBb g_bishopAttackBb;
extern RookAttackBb g_rookAttackBb;


class QueenAttackBb {
public:

	inline Bitboard QueenAttack(const Bitboard* thisBitboard, const Square sq) const
	{
		return g_rookAttackBb.RookAttack(thisBitboard, sq) | g_bishopAttackBb.BishopAttack(thisBitboard, sq);
	}

};