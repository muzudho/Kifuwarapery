#pragma once

#include "n160_100_bitboard.hpp"

//────────────────────────────────────────────────────────────────────────────────
// 桂
//────────────────────────────────────────────────────────────────────────────────
class KnightAttackBb {
public:
	Bitboard m_knightAttack[ColorNum][SquareNum];
	Bitboard m_knightCheckTable[ColorNum][SquareNum];

public:
	inline Bitboard KnightAttack(const Color c, const Square sq) const {
		return this->m_knightAttack[c][sq];
	}

	inline Bitboard KnightCheckTable(const Color c, const Square sq) const {
		return this->m_knightCheckTable[c][sq];
	}
};
