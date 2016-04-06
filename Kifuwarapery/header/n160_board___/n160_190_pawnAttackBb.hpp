#pragma once

#include "n160_100_bitboard.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class PawnAttackBb {
public:
	Bitboard m_controllBb[ColorNum][SquareNum];

public:

	void Initialize();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb[c][sq];
	}
};
