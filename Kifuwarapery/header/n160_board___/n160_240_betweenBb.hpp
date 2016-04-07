#pragma once

#include "n160_100_bitboard.hpp"

class BetweenBb {
private:
	Bitboard m_betweenBB_[SquareNum][SquareNum];

public:

	void Initialize();

	// sq1, sq2 の間(sq1, sq2 は含まない)のビットが立った Bitboard
	inline Bitboard GetBetweenBB(const Square sq1, const Square sq2) const {
		return this->m_betweenBB_[sq1][sq2];
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern BetweenBb g_betweenBb;
