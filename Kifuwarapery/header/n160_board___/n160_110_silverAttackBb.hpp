#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 銀
//────────────────────────────────────────────────────────────────────────────────
class SilverAttackBb {
private:
	Bitboard m_controllBb_[g_COLOR_NUM][SquareNum];
	Bitboard m_silverCheckTable_[g_COLOR_NUM][SquareNum];

public:

	void Initialize();
	void InitCheckTableSilver();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb_[c][sq];
	}

	inline Bitboard SilverCheckTable(const Color c, const Square sq) const {
		return this->m_silverCheckTable_[c][sq];
	}
};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern SilverAttackBb g_silverAttackBb;
