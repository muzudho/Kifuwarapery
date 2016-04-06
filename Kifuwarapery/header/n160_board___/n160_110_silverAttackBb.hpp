#pragma once

#include "../n160_board___/n160_100_bitboard.hpp"

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class SilverAttackBb {
public:
	Bitboard m_controllBb[ColorNum][SquareNum];

public:

	void Initialize();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb[c][sq];
	}

	Bitboard m_silverCheckTable[ColorNum][SquareNum];
	inline Bitboard SilverCheckTable(const Color c, const Square sq) const {
		return this->m_silverCheckTable[c][sq];
	}
};

