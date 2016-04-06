#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_110_silverAttackBb.hpp"

extern SilverAttackBb g_silverAttackBb;


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �p
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class BishopAttackBb {
public:
	Bitboard	m_controllBb[20224];
	int			m_controllBbIndex[SquareNum];
	Bitboard	m_bishopBlockMask[SquareNum];
	Bitboard	m_controllBbToEdge[SquareNum];

public:

#if defined FIND_MAGIC
	u64 findMagicBishop(const Square sqare);
#endif // #if defined FIND_MAGIC

	Bitboard BishopBlockMaskCalc(const Square square);
	Bitboard BishopAttackCalc(const Square square, const Bitboard& occupied);
	void InitBishopAttacks();

	// ��Q���������Ƃ��̗����� Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack ��ݒ肵�Ă���A���̊֐����ĂԂ��ƁB
	void InitializeToEdge();

	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return this->m_controllBbToEdge[sq];
	}

	// todo: �e�[�u������������
	inline Bitboard BishopStepAttacks(const Square sq) const {
		return g_silverAttackBb.GetControllBb(Black, sq) & g_silverAttackBb.GetControllBb(White, sq);
	}

	#if defined HAVE_BMI2
		inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq) const {
			const Bitboard block((*thisBitboard) & this->m_bishopBlockMask[sq]);
			return this->m_controllBb[this->m_controllBbIndex[sq] + OccupiedToIndex(block, this->m_bishopBlockMask[sq])];
		}
	#else
		inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq) const {
			const Bitboard block((*thisBitboard) & this->m_bishopBlockMask[sq]);
			return this->m_controllBb[this->m_controllBbIndex[sq] + block.OccupiedToIndex(ConfigBits::m_bishopMagic[sq], ConfigBits::m_bishopShiftBits[sq])];
		}
	#endif

};
