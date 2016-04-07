#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_120_bishopAttackBb.hpp"
#include "n160_160_kingAttackBb.hpp"

//extern KingAttackBb g_kingAttackBb;
//extern BishopAttackBb g_bishopAttackBb;


class HorseAttackBb {
public:
	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Square sq) const {
		return g_bishopAttackBb.BishopAttack(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}

	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return g_bishopAttackBb.GetControllBbToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}
};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern const HorseAttackBb g_horseAttackBb;
