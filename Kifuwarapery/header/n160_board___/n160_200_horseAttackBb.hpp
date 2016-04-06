#pragma once

#include "n160_100_bitboard.hpp"

extern KingAttackBb g_kingAttackBb;
extern BishopAttackBb g_bishopAttackBb;


class HorseAttackBb {
public:
	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Square sq) const {
		return g_bishopAttackBb.BishopAttack(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}

	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return g_bishopAttackBb.GetControllBbToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}
};
