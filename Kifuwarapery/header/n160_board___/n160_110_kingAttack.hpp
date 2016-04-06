#pragma once

#include "../../header/n160_board___/n160_100_bitboard.hpp"

//────────────────────────────────────────────────────────────────────────────────
// 玉
//────────────────────────────────────────────────────────────────────────────────
class KingAttackBb  {
public:
	// メモリ節約の為、1次元配列にして無駄が無いようにしている。
	Bitboard m_kingAttack[SquareNum];

public:

	// Bitboard で直接利きを返す関数。
	inline Bitboard KingAttack(const Square sq) {//const
		return this->m_kingAttack[sq];
	}
};//g_kingAttackBb
//KingAttackBb g_kingAttackBb;//const 