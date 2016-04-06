#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_120_silverAttack.hpp"

extern SilverAttackBb g_silverAttackBb;


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �p
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class BishopAttackBb {
public:
	Bitboard	m_bishopAttack[20224];
	int			m_bishopAttackIndex[SquareNum];
	Bitboard	m_bishopBlockMask[SquareNum];
	Bitboard	m_bishopAttackToEdge[SquareNum];

public:
	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return this->m_bishopAttackToEdge[sq];
	}

	// todo: �e�[�u������������
	inline Bitboard BishopStepAttacks(const Square sq) const {
		return g_silverAttackBb.SilverAttack(Black, sq) & g_silverAttackBb.SilverAttack(White, sq);
	}

};
