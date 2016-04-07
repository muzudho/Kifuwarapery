#pragma once

#include "n160_100_bitboard.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 玉の利き
//────────────────────────────────────────────────────────────────────────────────
const class KingAttackBb  {
private:
	// メモリ節約の為、1次元配列にして無駄が無いようにしている。
	Bitboard m_controllBb_[SquareNum]; // TODO: const にしたい。

public:

	void Initialize();

	// Bitboard で直接利きを返す関数。
	inline Bitboard GetControllBb(const Square sq) const {
		return this->m_controllBb_[sq];
	}
};


extern KingAttackBb g_kingAttackBb;
