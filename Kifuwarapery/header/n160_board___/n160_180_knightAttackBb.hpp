#pragma once

#include "n160_100_bitboard.hpp"

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �j
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class KnightAttackBb {
private:
	Bitboard m_controllBb_[ColorNum][SquareNum];
	Bitboard m_knightCheckTable_[ColorNum][SquareNum];

public:

	void Initialize();
	void InitCheckTableKnight();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb_[c][sq];
	}

	inline Bitboard KnightCheckTable(const Color c, const Square sq) const {
		return this->m_knightCheckTable_[c][sq];
	}
};


extern KnightAttackBb g_knightAttackBb;
