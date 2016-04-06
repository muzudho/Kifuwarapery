#pragma once

#include "n160_100_bitboard.hpp"

//class Bitboard;


class UtilBitboard {
public:
	//────────────────────────────────────────────────────────────────────────────────
	// 玉
	//────────────────────────────────────────────────────────────────────────────────
	// メモリ節約の為、1次元配列にして無駄が無いようにしている。
	static Bitboard m_kingAttack[SquareNum];

	// Bitboard で直接利きを返す関数。
	static inline Bitboard KingAttack(const Square sq) {
		return UtilBitboard::m_kingAttack[sq];
	}

	//────────────────────────────────────────────────────────────────────────────────
	// 角
	//────────────────────────────────────────────────────────────────────────────────
	static Bitboard m_bishopAttack[20224];
	static int		m_bishopAttackIndex[SquareNum];
	static Bitboard m_bishopBlockMask[SquareNum];
	static Bitboard m_bishopAttackToEdge[SquareNum];
	static inline Bitboard BishopAttackToEdge(const Square sq) { return UtilBitboard::m_bishopAttackToEdge[sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// 飛
	//────────────────────────────────────────────────────────────────────────────────
	// メモリ節約の為、1次元配列にして無駄が無いようにしている。
#if defined HAVE_BMI2
	static Bitboard m_rookAttack[495616];
#else
	static Bitboard m_rookAttack[512000];
#endif

	static Bitboard m_rookBlockMask[SquareNum];
	static Bitboard m_rookAttackToEdge[SquareNum];
	static inline Bitboard RookAttackToEdge(const Square sq) { return UtilBitboard::m_rookAttackToEdge[sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// 金
	//────────────────────────────────────────────────────────────────────────────────
	static Bitboard m_goldAttack[ColorNum][SquareNum];
	static inline Bitboard GoldAttack(const Color c, const Square sq) { return UtilBitboard::m_goldAttack[c][sq]; }

	static Bitboard m_goldCheckTable[ColorNum][SquareNum];

	static inline Bitboard GoldCheckTable(const Color c, const Square sq) { return UtilBitboard::m_goldCheckTable[c][sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// 銀
	//────────────────────────────────────────────────────────────────────────────────
	static Bitboard m_silverAttack[ColorNum][SquareNum];
	static inline Bitboard SilverAttack(const Color c, const Square sq) { return UtilBitboard::m_silverAttack[c][sq]; }

	static Bitboard m_silverCheckTable[ColorNum][SquareNum];
	static inline Bitboard SilverCheckTable(const Color c, const Square sq) { return UtilBitboard::m_silverCheckTable[c][sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// 桂
	//────────────────────────────────────────────────────────────────────────────────
	static Bitboard m_knightAttack[ColorNum][SquareNum];
	static inline Bitboard KnightAttack(const Color c, const Square sq) { return UtilBitboard::m_knightAttack[c][sq]; }

	static Bitboard m_knightCheckTable[ColorNum][SquareNum];
	static inline Bitboard KnightCheckTable(const Color c, const Square sq) { return UtilBitboard::m_knightCheckTable[c][sq]; }

	//────────────────────────────────────────────────────────────────────────────────
	// 香
	//────────────────────────────────────────────────────────────────────────────────
	// メモリ節約をせず、無駄なメモリを持っている。
	static Bitboard m_lanceAttack[ColorNum][SquareNum][128];

	// todo: 香車の筋がどこにあるか先に分かっていれば、Bitboard の片方の変数だけを調べれば良くなる。
	static inline Bitboard LanceAttack(const Bitboard* thisBitboard, const Color c, const Square sq) {//const 
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return UtilBitboard::m_lanceAttack[c][sq][index];
	}

	static Bitboard m_lanceAttackToEdge[ColorNum][SquareNum];

	static Bitboard m_lanceCheckTable[ColorNum][SquareNum];
	static inline Bitboard LanceCheckTable(const Color c, const Square sq) { return UtilBitboard::m_lanceCheckTable[c][sq]; }

	static inline Bitboard LanceAttackToEdge(const Color c, const Square sq) { return UtilBitboard::m_lanceAttackToEdge[c][sq]; }

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


	// 飛車の縦だけの利き。香車の利きを使い、index を共通化することで高速化している。
	static inline Bitboard RookAttackFile(const Bitboard* thisBitboard, const Square sq) {//const
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return UtilBitboard::m_lanceAttack[Black][sq][index] | UtilBitboard::m_lanceAttack[White][sq][index];
	}

	// todo: テーブル引きを検討
	static inline Bitboard RookStepAttacks(const Square sq) { return UtilBitboard::GoldAttack(Black, sq) & UtilBitboard::GoldAttack(White, sq); }
	// todo: テーブル引きを検討
	static inline Bitboard BishopStepAttacks(const Square sq) { return UtilBitboard::SilverAttack(Black, sq) & UtilBitboard::SilverAttack(White, sq); }
	// 前方3方向の位置のBitboard
	static inline Bitboard GoldAndSilverAttacks(const Color c, const Square sq) { return UtilBitboard::GoldAttack(c, sq) & UtilBitboard::SilverAttack(c, sq); }


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
			return UtilBitboard::m_rookAttack[ConfigBits::m_rookAttackIndex[sq] + block.OccupiedToIndex(ConfigBits::m_rookMagic[sq], ConfigBits::m_rookShiftBits[sq])];
		}

		static inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq) {//const
			const Bitboard block((*thisBitboard) & UtilBitboard::m_bishopBlockMask[sq]);
			return UtilBitboard::m_bishopAttack[UtilBitboard::m_bishopAttackIndex[sq] + block.OccupiedToIndex(ConfigBits::m_bishopMagic[sq], ConfigBits::m_bishopShiftBits[sq])];
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
		return UtilBitboard::RookAttackToEdge(sq) | UtilBitboard::KingAttack(sq);
	}
	static inline Bitboard HorseAttackToEdge(const Square sq) {
		return UtilBitboard::BishopAttackToEdge(sq) | UtilBitboard::KingAttack(sq);
	}

};

