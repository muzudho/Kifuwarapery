#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_120_bishopAttackBb.hpp"
#include "n160_150_rookAttackBb.hpp"

//extern BishopAttackBb g_bishopAttackBb;
//extern RookAttackBb g_rookAttackBb;


class QueenAttackBb {
public:

	inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Square sq) const
	{
		return g_rookAttackBb.GetControllBb(thisBitboard, sq) | g_bishopAttackBb.BishopAttack(thisBitboard, sq);
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern const QueenAttackBb g_queenAttackBb;
