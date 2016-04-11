#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_150_rookAttackBb.hpp"
#include "n160_160_kingAttackBb.hpp"



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


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern const DragonAttackBb g_dragonAttackBb;
