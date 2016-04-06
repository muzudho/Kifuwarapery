#pragma once

#include "n160_100_bitboard.hpp"

//────────────────────────────────────────────────────────────────────────────────
// 桂
//────────────────────────────────────────────────────────────────────────────────
class KnightAttackBb {
public:
	Bitboard m_controllBb[ColorNum][SquareNum];
	Bitboard m_knightCheckTable[ColorNum][SquareNum];

public:

	void Initialize();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb[c][sq];
	}

	inline Bitboard KnightCheckTable(const Color c, const Square sq) const {
		return this->m_knightCheckTable[c][sq];
	}
};
