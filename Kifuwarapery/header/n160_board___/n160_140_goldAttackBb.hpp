#pragma once

#include "n160_100_bitboard.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 金
//────────────────────────────────────────────────────────────────────────────────
class GoldAttackBb {
private:
	Bitboard m_controllBb_[g_COLOR_NUM][SquareNum];
	Bitboard m_goldCheckTable_[g_COLOR_NUM][SquareNum];

public:

	void Initialize();
	void InitCheckTableGold();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb_[c][sq];
	}

	inline Bitboard GoldCheckTable(const Color c, const Square sq) const {
		return this->m_goldCheckTable_[c][sq];
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern GoldAttackBb g_goldAttackBb;
