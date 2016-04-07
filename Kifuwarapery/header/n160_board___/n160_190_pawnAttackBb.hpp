#pragma once

#include "n160_100_bitboard.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class PawnAttackBb {
private:
	Bitboard m_controllBb_[ColorNum][SquareNum];

public:

	void Initialize();

	inline Bitboard GetControllBb(const Color c, const Square sq) const {
		return this->m_controllBb_[c][sq];
	}

};


extern PawnAttackBb g_pawnAttackBb;

