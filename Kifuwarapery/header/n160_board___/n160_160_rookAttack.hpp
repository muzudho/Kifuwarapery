#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_140_lanceAttack.hpp"
#include "n160_150_goldAttack.hpp"

extern LanceAttackBb g_lanceAttackBb;
extern GoldAttackBb g_goldAttackBb;


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class RookAttackBb {
public:
	// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
#if defined HAVE_BMI2
	Bitboard m_rookAttack[495616];
#else
	Bitboard m_rookAttack[512000];
#endif

	Bitboard m_rookBlockMask[SquareNum];
	Bitboard m_rookAttackToEdge[SquareNum];

public:
	inline Bitboard RookAttackToEdge(const Square sq) const {
		return this->m_rookAttackToEdge[sq];
	}

	// ��Ԃ̏c�����̗����B���Ԃ̗������g���Aindex �����ʉ����邱�Ƃō��������Ă���B
	inline Bitboard RookAttackFile(const Bitboard* thisBitboard, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return g_lanceAttackBb.m_controllBb[Black][sq][index] | g_lanceAttackBb.m_controllBb[White][sq][index];
	}

	// todo: �e�[�u������������
	inline Bitboard RookStepAttacks(const Square sq) const {
		return g_goldAttackBb.GoldAttack(Black, sq) & g_goldAttackBb.GoldAttack(White, sq);
	}
};

