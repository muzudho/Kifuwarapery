#pragma once

#include "n160_100_bitboard.hpp"

class BetweenBb {
public:
	Bitboard m_betweenBB[SquareNum][SquareNum];

public:
	// sq1, sq2 ‚ÌŠÔ(sq1, sq2 ‚ÍŠÜ‚Ü‚È‚¢)‚Ìƒrƒbƒg‚ª—§‚Á‚½ Bitboard
	inline Bitboard BetweenBB(const Square sq1, const Square sq2) const {
		return this->m_betweenBB[sq1][sq2];
	}

};
