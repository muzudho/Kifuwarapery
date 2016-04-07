#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_110_silverAttackBb.hpp"
#include "n160_140_goldAttackBb.hpp"

//extern SilverAttackBb g_silverAttackBb;
//extern GoldAttackBb g_goldAttackBb;


class GoldAndSilverAttackBb {
public:

	// 前方3方向の位置のBitboard
	inline Bitboard GoldAndSilverAttacks(const Color c, const Square sq) const {
		return g_goldAttackBb.GetControllBb(c, sq) & g_silverAttackBb.GetControllBb(c, sq);
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern const GoldAndSilverAttackBb g_goldAndSilverAttackBb;
