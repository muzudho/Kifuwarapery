#pragma once

#include "../../header/n160_board___/n160_100_bitboard.hpp"

//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class KingAttackBb  {
public:
	// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
	Bitboard m_kingAttack[SquareNum];

public:

	// Bitboard �Œ��ڗ�����Ԃ��֐��B
	inline Bitboard KingAttack(const Square sq) {//const
		return this->m_kingAttack[sq];
	}
};//g_kingAttackBb
//KingAttackBb g_kingAttackBb;//const 