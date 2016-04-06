#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_120_silverAttack.hpp"
#include "n160_150_goldAttack.hpp"

extern SilverAttackBb g_silverAttackBb;
extern GoldAttackBb g_goldAttackBb;


class GoldAndSilverAttackBb {
public:

	// 前方3方向の位置のBitboard
	inline Bitboard GoldAndSilverAttacks(const Color c, const Square sq) const {
		return g_goldAttackBb.GoldAttack(c, sq) & g_silverAttackBb.SilverAttack(c, sq);
	}

};