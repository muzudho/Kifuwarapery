#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_110_kingAttack.hpp"
#include "n160_120_silverAttack.hpp"
#include "n160_130_bishopAttack.hpp"
#include "n160_140_lanceAttack.hpp"
#include "n160_150_goldAttack.hpp"
#include "n160_160_rookAttack.hpp"
#include "n160_170_goldAndSilverAttack.hpp"
#include "n160_180_knightAttack.hpp"
#include "n160_190_pawnAttack.hpp"

extern KingAttackBb g_kingAttackBb;
extern SilverAttackBb g_silverAttackBb;
extern BishopAttackBb g_bishopAttackBb;
extern LanceAttackBb g_lanceAttackBb;
extern GoldAttackBb g_goldAttackBb;
extern RookAttackBb g_rookAttackBb;
extern GoldAndSilverAttackBb g_goldAndSilverAttackBb;
extern KnightAttackBb g_knightAttackBb;
extern PawnAttackBb g_pawnAttackBb;


class UtilBitboard {
public:

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

	// for debug
	static void PrintBoard(const Bitboard* thisBitboard);//const

public:



	static bool IsSet(const Bitboard* thisBitboard, const Square sq);// const;

	static void SetBit(Bitboard* thisBitboard, const Square sq);




	static inline Bitboard DragonAttack(const Bitboard* thisBitboard, const Square sq) //const
	{
		return g_rookAttackBb.RookAttack(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}

	static inline Bitboard QueenAttack(const Bitboard* thisBitboard, const Square sq) //const
	{
		return g_rookAttackBb.RookAttack(thisBitboard, sq) | g_bishopAttackBb.BishopAttack(thisBitboard, sq);
	}

	static inline Bitboard HorseAttack(const Bitboard* thisBitboard, const Square sq) {//const
		return g_bishopAttackBb.BishopAttack(thisBitboard, sq) | g_kingAttackBb.GetControllBb(sq);
	}



	static inline Bitboard DragonAttackToEdge(const Square sq) {
		return g_rookAttackBb.RookAttackToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}
	static inline Bitboard HorseAttackToEdge(const Square sq) {
		return g_bishopAttackBb.GetControllBbToEdge(sq) | g_kingAttackBb.GetControllBb(sq);
	}

};

