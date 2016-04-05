#pragma once

#include "n160_100_bitboard.hpp"

class UtilBitboard {
public:
	static Bitboard m_kingAttack[SquareNum];

	// Bitboard で直接利きを返す関数。
	static inline Bitboard KingAttack(const Square sq) {
		return UtilBitboard::m_kingAttack[sq];
	}

public:

	// メモリ節約の為、1次元配列にして無駄が無いようにしている。
	#if defined HAVE_BMI2
		static Bitboard m_rookAttack[495616];
	#else
		static Bitboard m_rookAttack[512000];
	#endif


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
			const Bitboard block((*thisBitboard) & g_rookBlockMask[sq]);
			return UtilBitboard::m_rookAttack[ConfigBits::m_rookAttackIndex[sq] + block.OccupiedToIndex(ConfigBits::m_rookMagic[sq], ConfigBits::m_rookShiftBits[sq])];
		}

		static inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq) {//const
			const Bitboard block((*thisBitboard) & g_bishopBlockMask[sq]);
			return g_bishopAttack[g_bishopAttackIndex[sq] + block.OccupiedToIndex(ConfigBits::m_bishopMagic[sq], ConfigBits::m_bishopShiftBits[sq])];
		}

	#endif


	static inline Bitboard DragonAttack(const Bitboard* thisBitboard, const Square sq) //const
	{
		return UtilBitboard::RookAttack(thisBitboard, sq) | UtilBitboard::KingAttack(sq);
	}

	static inline Bitboard QueenAttack(const Bitboard* thisBitboard, const Square sq) //const
	{
		return UtilBitboard::RookAttack(thisBitboard, sq) | UtilBitboard::BishopAttack(thisBitboard, sq);
	}

	static inline Bitboard HorseAttack(const Bitboard* thisBitboard, const Square sq) {//const
		return UtilBitboard::BishopAttack(thisBitboard, sq) | UtilBitboard::KingAttack(sq);
	}



	static inline Bitboard DragonAttackToEdge(const Square sq) {
		return Bitboard::RookAttackToEdge(sq) | UtilBitboard::KingAttack(sq);
	}
	static inline Bitboard HorseAttackToEdge(const Square sq) {
		return Bitboard::BishopAttackToEdge(sq) | UtilBitboard::KingAttack(sq);
	}

};

