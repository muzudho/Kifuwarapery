#pragma once

#include "n160_100_bitboard.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class GoldAttackBb {
public:
	Bitboard m_goldAttack[ColorNum][SquareNum];
	Bitboard m_goldCheckTable[ColorNum][SquareNum];

public:
	inline Bitboard GoldAttack(const Color c, const Square sq) const {
		return this->m_goldAttack[c][sq];
	}

	inline Bitboard GoldCheckTable(const Color c, const Square sq) const {
		return this->m_goldCheckTable[c][sq];
	}

};

