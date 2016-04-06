#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_110_kingAttack.hpp"
#include "n160_120_silverAttack.hpp"
#include "n160_130_bishopAttack.hpp"
#include "n160_140_lanceAttack.hpp"
#include "n160_150_goldAttack.hpp"
#include "n160_160_rookAttack.hpp"

extern KingAttackBb g_kingAttackBb;
extern SilverAttackBb g_silverAttackBb;
extern BishopAttackBb g_bishopAttackBb;
extern LanceAttackBb g_lanceAttackBb;
extern GoldAttackBb g_goldAttackBb;
extern RookAttackBb g_rookAttackBb;


class UtilBitboard {
public:


	//────────────────────────────────────────────────────────────────────────────────
	// 桂
	//────────────────────────────────────────────────────────────────────────────────
	static Bitboard m_knightAttack[ColorNum][SquareNum];
	static inline Bitboard KnightAttack(const Color c, const Square sq) { return UtilBitboard::m_knightAttack[c][sq]; }

	static Bitboard m_knightCheckTable[ColorNum][SquareNum];
	static inline Bitboard KnightCheckTable(const Color c, const Square sq) { return UtilBitboard::m_knightCheckTable[c][sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// 歩
	//────────────────────────────────────────────────────────────────────────────────
	static Bitboard m_pawnAttack[ColorNum][SquareNum];
	static inline Bitboard PawnAttack(const Color c, const Square sq) { return UtilBitboard::m_pawnAttack[c][sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// マスク
	//────────────────────────────────────────────────────────────────────────────────
	static const Bitboard m_setMaskBB[SquareNum];
	inline static Bitboard SetMaskBB(const Square sq) { return UtilBitboard::m_setMaskBB[sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// 論理演算
	//────────────────────────────────────────────────────────────────────────────────
	static void ClearBit(Bitboard* thisBitboard, const Square sq);

	static void XorBit(Bitboard* thisBitboard, const Square sq);

	static void XorBit(Bitboard* thisBitboard, const Square sq1, const Square sq2);

	//────────────────────────────────────────────────────────────────────────────────
	// その他
	//────────────────────────────────────────────────────────────────────────────────
	static Bitboard m_betweenBB[SquareNum][SquareNum];
	// sq1, sq2 の間(sq1, sq2 は含まない)のビットが立った Bitboard
	static inline Bitboard BetweenBB(const Square sq1, const Square sq2) { return UtilBitboard::m_betweenBB[sq1][sq2]; }


	// 前方3方向の位置のBitboard
	static inline Bitboard GoldAndSilverAttacks(const Color c, const Square sq) {
		return g_goldAttackBb.GoldAttack(c, sq) & g_silverAttackBb.SilverAttack(c, sq);
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
			const Bitboard block((*thisBitboard) & g_rookAttackBb.m_rookBlockMask[sq]);
			return g_rookAttackBb.m_rookAttack[
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
		return g_rookAttackBb.RookAttackToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}
	static inline Bitboard HorseAttackToEdge(const Square sq) {
		return g_bishopAttackBb.GetControllBbToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}

};

