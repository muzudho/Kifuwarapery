#pragma once

#include "n160_100_bitboard.hpp"

class BetweenBb {
public:
	Bitboard m_betweenBB[SquareNum][SquareNum];

public:
	// sq1, sq2 �̊�(sq1, sq2 �͊܂܂Ȃ�)�̃r�b�g�������� Bitboard
	inline Bitboard BetweenBB(const Square sq1, const Square sq2) const {
		return this->m_betweenBB[sq1][sq2];
	}

};
