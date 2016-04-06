#pragma once

#include "n160_100_bitboard.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class PawnAttackBb {
public:
	Bitboard m_pawnAttack[ColorNum][SquareNum];

public:
	inline Bitboard PawnAttack(const Color c, const Square sq) const {
		return this->m_pawnAttack[c][sq];
	}
};
