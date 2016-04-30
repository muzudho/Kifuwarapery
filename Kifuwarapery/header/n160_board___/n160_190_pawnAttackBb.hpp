#pragma once

#include "n160_100_bitboard.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 歩
//────────────────────────────────────────────────────────────────────────────────
class PawnAttackBb {
private:
	Bitboard m_controllBb_[g_COLOR_NUM][SquareNum];

public:

	void Initialize();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb_[c][sq];
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern PawnAttackBb g_pawnAttackBb;

