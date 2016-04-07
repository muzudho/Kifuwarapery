#pragma once

#include "n160_050_configBits.hpp"
#include "n160_100_bitboard.hpp"
#include "n160_130_lanceAttackBb.hpp"
#include "n160_140_goldAttackBb.hpp"

extern ConfigBits g_configBits;
extern LanceAttackBb g_lanceAttackBb;
extern GoldAttackBb g_goldAttackBb;


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ��
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
class RookAttackBb {
private:
	// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
#if defined HAVE_BMI2
	Bitboard m_controllBb_[495616];
#else
	Bitboard m_controllBb_[512000];
#endif

	Bitboard m_rookBlockMask_[SquareNum];
	Bitboard m_controllBbToEdge_[SquareNum];



public:

#if defined FIND_MAGIC
	u64 findMagicRook(const Square sqare);
#endif // #if defined FIND_MAGIC

	// �������p
	Bitboard RookBlockMaskCalc(const Square square) const;
	Bitboard RookAttackCalc(const Square square, const Bitboard& occupied) const;
	void InitRookAttacks();

	// ��Q���������Ƃ��̗����� Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack ��ݒ肵�Ă���A���̊֐����ĂԂ��ƁB
	void InitializeToEdge();

	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return this->m_controllBbToEdge_[sq];
	}

	// ��Ԃ̏c�����̗����B���Ԃ̗������g���Aindex �����ʉ����邱�Ƃō��������Ă���B
	inline Bitboard GetControllBbFile(const Bitboard* thisBitboard, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> g_configBits.m_slide[sq]) & 127;
		return g_lanceAttackBb.m_controllBb[Black][sq][index] | g_lanceAttackBb.m_controllBb[White][sq][index];
	}

	// todo: �e�[�u������������
	inline Bitboard RookStepAttacks(const Square sq) const {
		return g_goldAttackBb.GetControllBb(Black, sq) & g_goldAttackBb.GetControllBb(White, sq);
	}

	#if defined HAVE_BMI2
		inline Bitboard GetControllBb(Bitboard& thisBitboard, const Square sq) const {
			const Bitboard block(thisBitboard & this->m_rookBlockMask_[sq]);
			return this->m_controllBb_[this->m_rookAttackIndex[sq] + OccupiedToIndex(block, this->m_rookBlockMask_[sq])];
		}
	#else
		inline Bitboard GetControllBb(const Bitboard* thisBitboard, const Square sq) const {
			const Bitboard block((*thisBitboard) & this->m_rookBlockMask_[sq]);
			return this->m_controllBb_[
				g_configBits.m_rookAttackIndex[sq] +
					block.OccupiedToIndex(g_configBits.m_rookMagic[sq], g_configBits.m_rookShiftBits[sq])
			];
		}
	#endif

};

