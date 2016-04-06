#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_110_kingAttack.hpp"
#include "n160_120_bishopAttack.hpp"
#include "n160_130_lanceAttack.hpp"

extern KingAttackBb g_kingAttackBb;
extern BishopAttackBb g_bishopAttackBb;
extern LanceAttackBb g_lanceAttackBb;


class UtilBitboard {
public:


	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ��
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
#if defined HAVE_BMI2
	static Bitboard m_rookAttack[495616];
#else
	static Bitboard m_rookAttack[512000];
#endif

	static Bitboard m_rookBlockMask[SquareNum];
	static Bitboard m_rookAttackToEdge[SquareNum];
	static inline Bitboard RookAttackToEdge(const Square sq) { return UtilBitboard::m_rookAttackToEdge[sq]; }

	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ��
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	static Bitboard m_goldAttack[ColorNum][SquareNum];
	static inline Bitboard GoldAttack(const Color c, const Square sq) { return UtilBitboard::m_goldAttack[c][sq]; }

	static Bitboard m_goldCheckTable[ColorNum][SquareNum];

	static inline Bitboard GoldCheckTable(const Color c, const Square sq) { return UtilBitboard::m_goldCheckTable[c][sq]; }

	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ��
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	static Bitboard m_silverAttack[ColorNum][SquareNum];
	static inline Bitboard SilverAttack(const Color c, const Square sq) { return UtilBitboard::m_silverAttack[c][sq]; }

	static Bitboard m_silverCheckTable[ColorNum][SquareNum];
	static inline Bitboard SilverCheckTable(const Color c, const Square sq) { return UtilBitboard::m_silverCheckTable[c][sq]; }

	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// �j
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	static Bitboard m_knightAttack[ColorNum][SquareNum];
	static inline Bitboard KnightAttack(const Color c, const Square sq) { return UtilBitboard::m_knightAttack[c][sq]; }

	static Bitboard m_knightCheckTable[ColorNum][SquareNum];
	static inline Bitboard KnightCheckTable(const Color c, const Square sq) { return UtilBitboard::m_knightCheckTable[c][sq]; }

	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ��
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	static Bitboard m_pawnAttack[ColorNum][SquareNum];
	static inline Bitboard PawnAttack(const Color c, const Square sq) { return UtilBitboard::m_pawnAttack[c][sq]; }

	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// �}�X�N
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	static const Bitboard m_setMaskBB[SquareNum];
	inline static Bitboard SetMaskBB(const Square sq) { return UtilBitboard::m_setMaskBB[sq]; }

	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// �_�����Z
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	static void ClearBit(Bitboard* thisBitboard, const Square sq);

	static void XorBit(Bitboard* thisBitboard, const Square sq);

	static void XorBit(Bitboard* thisBitboard, const Square sq1, const Square sq2);

	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	// ���̑�
	//����������������������������������������������������������������������������������������������������������������������������������������������������������������
	static Bitboard m_betweenBB[SquareNum][SquareNum];
	// sq1, sq2 �̊�(sq1, sq2 �͊܂܂Ȃ�)�̃r�b�g�������� Bitboard
	static inline Bitboard BetweenBB(const Square sq1, const Square sq2) { return UtilBitboard::m_betweenBB[sq1][sq2]; }


	// ��Ԃ̏c�����̗����B���Ԃ̗������g���Aindex �����ʉ����邱�Ƃō��������Ă���B
	static inline Bitboard RookAttackFile(const Bitboard* thisBitboard, const Square sq) {//const
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return g_lanceAttackBb.m_controllBb[Black][sq][index] | g_lanceAttackBb.m_controllBb[White][sq][index];
	}

	// todo: �e�[�u������������
	static inline Bitboard RookStepAttacks(const Square sq) {
		return UtilBitboard::GoldAttack(Black, sq) & UtilBitboard::GoldAttack(White, sq);
	}
	// todo: �e�[�u������������
	static inline Bitboard BishopStepAttacks(const Square sq) {
		return UtilBitboard::SilverAttack(Black, sq) & UtilBitboard::SilverAttack(White, sq);
	}
	// �O��3�����̈ʒu��Bitboard
	static inline Bitboard GoldAndSilverAttacks(const Color c, const Square sq) {
		return UtilBitboard::GoldAttack(c, sq) & UtilBitboard::SilverAttack(c, sq);
	}


	// for debug
	static void PrintBoard(const Bitboard* thisBitboard);//const

public:



	static bool IsSet(const Bitboard* thisBitboard, const Square sq);// const;

	static void SetBit(Bitboard* thisBitboard, const Square sq);


	#if defined HAVE_BMI2
		static inline Bitboard RookAttack(Bitboard& thisBitboard, const Square sq) {// const
			const Bitboard block(thisBitboard & g_rookBlockMask[sq]);
			return g_rookAttack[g_rookAttackIndex[sq] + OccupiedToIndex(block, g_rookBlockMask[sq])];
		}

		static inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq)  {//const
			const Bitboard block((*thisBitboard) & g_bishopBlockMask[sq]);
			return g_bishopAttack[g_bishopAttackIndex[sq] + OccupiedToIndex(block, g_bishopBlockMask[sq])];
		}

	#else
		static inline Bitboard RookAttack(const Bitboard* thisBitboard, const Square sq) {// const
			const Bitboard block((*thisBitboard) & UtilBitboard::m_rookBlockMask[sq]);
			return UtilBitboard::m_rookAttack[
				ConfigBits::m_rookAttackIndex[sq] +
				block.OccupiedToIndex(ConfigBits::m_rookMagic[sq], ConfigBits::m_rookShiftBits[sq])
			];
		}

		static inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq) {//const
			const Bitboard block((*thisBitboard) & g_bishopAttackBb.m_bishopBlockMask[sq]);
			return g_bishopAttackBb.m_bishopAttack[g_bishopAttackBb.m_bishopAttackIndex[sq] + block.OccupiedToIndex(ConfigBits::m_bishopMagic[sq], ConfigBits::m_bishopShiftBits[sq])];
		}

	#endif


	static inline Bitboard DragonAttack(const Bitboard* thisBitboard, const Square sq) //const
	{
		return UtilBitboard::RookAttack(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}

	static inline Bitboard QueenAttack(const Bitboard* thisBitboard, const Square sq) //const
	{
		return UtilBitboard::RookAttack(thisBitboard, sq) | UtilBitboard::BishopAttack(thisBitboard, sq);
	}

	static inline Bitboard HorseAttack(const Bitboard* thisBitboard, const Square sq) {//const
		return UtilBitboard::BishopAttack(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}



	static inline Bitboard DragonAttackToEdge(const Square sq) {
		return UtilBitboard::RookAttackToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}
	static inline Bitboard HorseAttackToEdge(const Square sq) {
		return g_bishopAttackBb.GetControllBbToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}

};

