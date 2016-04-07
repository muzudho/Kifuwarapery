#pragma once

#include "n160_100_bitboard.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �ʂ̗���
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
const class KingAttackBb  {
private:
	// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
	Bitboard m_controllBb_[SquareNum]; // TODO: const �ɂ������B

public:

	void Initialize();

	// Bitboard �Œ��ڗ�����Ԃ��֐��B
	inline Bitboard GetControllBb(const Square sq) const {
		return this->m_controllBb_[sq];
	}
};


extern KingAttackBb g_kingAttackBb;
