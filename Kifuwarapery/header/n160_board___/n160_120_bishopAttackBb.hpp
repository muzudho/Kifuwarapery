#pragma once


#include "n160_100_bitboard.hpp"
#include "n160_110_silverAttackBb.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 角
//────────────────────────────────────────────────────────────────────────────────
class BishopAttackBb {
private:


	// 各マスのbishopが利きを調べる必要があるマスの数
	const int m_bishopBlockBits[SquareNum] = {
		7,  6,  6,  6,  6,  6,  6,  6,  7,
		6,  6,  6,  6,  6,  6,  6,  6,  6,
		6,  6,  8,  8,  8,  8,  8,  6,  6,
		6,  6,  8, 10, 10, 10,  8,  6,  6,
		6,  6,  8, 10, 12, 10,  8,  6,  6,
		6,  6,  8, 10, 10, 10,  8,  6,  6,
		6,  6,  8,  8,  8,  8,  8,  6,  6,
		6,  6,  6,  6,  6,  6,  6,  6,  6,
		7,  6,  6,  6,  6,  6,  6,  6,  7
	};

	// Magic Bitboard で利きを求める際のシフト量
	const int m_bishopShiftBits[SquareNum] = {
		57, 58, 58, 58, 58, 58, 58, 58, 57,
		58, 58, 58, 58, 58, 58, 58, 58, 58,
		58, 58, 56, 56, 56, 56, 56, 58, 58,
		58, 58, 56, 54, 54, 54, 56, 58, 58,
		58, 58, 56, 54, 52, 54, 56, 58, 58,
		58, 58, 56, 54, 54, 54, 56, 58, 58,
		58, 58, 56, 56, 56, 56, 56, 58, 58,
		58, 58, 58, 58, 58, 58, 58, 58, 58,
		57, 58, 58, 58, 58, 58, 58, 58, 57
	};

#if defined HAVE_BMI2
#else
	const u64 m_bishopMagic[SquareNum] = {
		UINT64_C(0x20101042c8200428), UINT64_C(0x840240380102),     UINT64_C(0x800800c018108251),
		UINT64_C(0x82428010301000),   UINT64_C(0x481008201000040),  UINT64_C(0x8081020420880800),
		UINT64_C(0x804222110000),     UINT64_C(0xe28301400850),     UINT64_C(0x2010221420800810),
		UINT64_C(0x2600010028801824), UINT64_C(0x8048102102002),    UINT64_C(0x4000248100240402),
		UINT64_C(0x49200200428a2108), UINT64_C(0x460904020844),     UINT64_C(0x2001401020830200),
		UINT64_C(0x1009008120),       UINT64_C(0x4804064008208004), UINT64_C(0x4406000240300ca0),
		UINT64_C(0x222001400803220),  UINT64_C(0x226068400182094),  UINT64_C(0x95208402010d0104),
		UINT64_C(0x4000807500108102), UINT64_C(0xc000200080500500), UINT64_C(0x5211000304038020),
		UINT64_C(0x1108100180400820), UINT64_C(0x10001280a8a21040), UINT64_C(0x100004809408a210),
		UINT64_C(0x202300002041112),  UINT64_C(0x4040a8000460408),  UINT64_C(0x204020021040201),
		UINT64_C(0x8120013180404),    UINT64_C(0xa28400800d020104), UINT64_C(0x200c201000604080),
		UINT64_C(0x1082004000109408), UINT64_C(0x100021c00c410408), UINT64_C(0x880820905004c801),
		UINT64_C(0x1054064080004120), UINT64_C(0x30c0a0224001030),  UINT64_C(0x300060100040821),
		UINT64_C(0x51200801020c006),  UINT64_C(0x2100040042802801), UINT64_C(0x481000820401002),
		UINT64_C(0x40408a0450000801), UINT64_C(0x810104200000a2),   UINT64_C(0x281102102108408),
		UINT64_C(0x804020040280021),  UINT64_C(0x2420401200220040), UINT64_C(0x80010144080c402),
		UINT64_C(0x80104400800002),   UINT64_C(0x1009048080400081), UINT64_C(0x100082000201008c),
		UINT64_C(0x10001008080009),   UINT64_C(0x2a5006b80080004),  UINT64_C(0xc6288018200c2884),
		UINT64_C(0x108100104200a000), UINT64_C(0x141002030814048),  UINT64_C(0x200204080010808),
		UINT64_C(0x200004013922002),  UINT64_C(0x2200000020050815), UINT64_C(0x2011010400040800),
		UINT64_C(0x1020040004220200), UINT64_C(0x944020104840081),  UINT64_C(0x6080a080801c044a),
		UINT64_C(0x2088400811008020), UINT64_C(0xc40aa04208070),    UINT64_C(0x4100800440900220),
		UINT64_C(0x48112050),         UINT64_C(0x818200d062012a10), UINT64_C(0x402008404508302),
		UINT64_C(0x100020101002),     UINT64_C(0x20040420504912),   UINT64_C(0x2004008118814),
		UINT64_C(0x1000810650084024), UINT64_C(0x1002a03002408804), UINT64_C(0x2104294801181420),
		UINT64_C(0x841080240500812),  UINT64_C(0x4406009000004884), UINT64_C(0x80082004012412),
		UINT64_C(0x80090880808183),   UINT64_C(0x300120020400410),  UINT64_C(0x21a090100822002)
	};
#endif

	Bitboard	m_controllBb_[20224];
	int			m_controllBbIndex_[SquareNum];
	Bitboard	m_bishopBlockMask_[SquareNum];
	Bitboard	m_controllBbToEdge_[SquareNum];

public:

#if defined FIND_MAGIC
	u64 findMagicBishop(const Square sqare);
#endif // #if defined FIND_MAGIC

	Bitboard BishopBlockMaskCalc(const Square square) const;
	Bitboard BishopAttackCalc(const Square square, const Bitboard& occupied) const;
	void InitBishopAttacks();

	// 障害物が無いときの利きの Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack を設定してから、この関数を呼ぶこと。
	void InitializeToEdge();

	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return this->m_controllBbToEdge_[sq];
	}

	// todo: テーブル引きを検討
	inline Bitboard BishopStepAttacks(const Square sq) const {
		return g_silverAttackBb.GetControllBb(Black, sq) & g_silverAttackBb.GetControllBb(White, sq);
	}

	#if defined HAVE_BMI2
		inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq) const {
			const Bitboard block((*thisBitboard) & this->m_bishopBlockMask_[sq]);
			return this->m_controllBb_[this->m_controllBbIndex_[sq] + OccupiedToIndex(block, this->m_bishopBlockMask_[sq])];
		}
	#else
		inline Bitboard BishopAttack(const Bitboard* thisBitboard, const Square sq) const {
			const Bitboard block((*thisBitboard) & this->m_bishopBlockMask_[sq]);
			return this->m_controllBb_[
				this->m_controllBbIndex_[sq] +
				block.OccupiedToIndex(this->m_bishopMagic[sq], this->m_bishopShiftBits[sq])
			];
		}
	#endif

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern BishopAttackBb g_bishopAttackBb;
