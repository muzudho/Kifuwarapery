#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_130_lanceAttackBb.hpp"
#include "n160_140_goldAttackBb.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 飛
//────────────────────────────────────────────────────────────────────────────────
class RookAttackBb {
private:


	// 各マスのrookが利きを調べる必要があるマスの数
	const int m_rookBlockBits_[SquareNum] = {
		14, 13, 13, 13, 13, 13, 13, 13, 14,
		13, 12, 12, 12, 12, 12, 12, 12, 13,
		13, 12, 12, 12, 12, 12, 12, 12, 13,
		13, 12, 12, 12, 12, 12, 12, 12, 13,
		13, 12, 12, 12, 12, 12, 12, 12, 13,
		13, 12, 12, 12, 12, 12, 12, 12, 13,
		13, 12, 12, 12, 12, 12, 12, 12, 13,
		13, 12, 12, 12, 12, 12, 12, 12, 13,
		14, 13, 13, 13, 13, 13, 13, 13, 14
	};

	// Magic Bitboard で利きを求める際のシフト量
	// g_rookShiftBits[17], g_rookShiftBits[53] はマジックナンバーが見つからなかったため、
	// シフト量を 1 つ減らす。(テーブルサイズを 2 倍にする。)
	// この方法は issei_y さんに相談したところ、教えて頂いた方法。
	// PEXT Bitboardを使用する際はシフト量を減らす必要が無い。
	const int m_rookShiftBits_[SquareNum] = {
		50, 51, 51, 51, 51, 51, 51, 51, 50,
#if defined HAVE_BMI2
		51, 52, 52, 52, 52, 52, 52, 52, 51,
#else
		51, 52, 52, 52, 52, 52, 52, 52, 50, // [17]: 51 -> 50
#endif
		51, 52, 52, 52, 52, 52, 52, 52, 51,
		51, 52, 52, 52, 52, 52, 52, 52, 51,
		51, 52, 52, 52, 52, 52, 52, 52, 51,
#if defined HAVE_BMI2
		51, 52, 52, 52, 52, 52, 52, 52, 51,
#else
		51, 52, 52, 52, 52, 52, 52, 52, 50, // [53]: 51 -> 50
#endif
		51, 52, 52, 52, 52, 52, 52, 52, 51,
		51, 52, 52, 52, 52, 52, 52, 52, 51,
		50, 51, 51, 51, 51, 51, 51, 51, 50
	};

#if defined HAVE_BMI2
#else
	const u64 m_rookMagic_[SquareNum] = {
		UINT64_C(0x140000400809300),  UINT64_C(0x1320000902000240), UINT64_C(0x8001910c008180),
		UINT64_C(0x40020004401040),   UINT64_C(0x40010000d01120),   UINT64_C(0x80048020084050),
		UINT64_C(0x40004000080228),   UINT64_C(0x400440000a2a0a),   UINT64_C(0x40003101010102),
		UINT64_C(0x80c4200012108100), UINT64_C(0x4010c00204000c01), UINT64_C(0x220400103250002),
		UINT64_C(0x2600200004001),    UINT64_C(0x40200052400020),   UINT64_C(0xc00100020020008),
		UINT64_C(0x9080201000200004), UINT64_C(0x2200201000080004), UINT64_C(0x80804c0020200191),
		UINT64_C(0x45383000009100),   UINT64_C(0x30002800020040),   UINT64_C(0x40104000988084),
		UINT64_C(0x108001000800415),  UINT64_C(0x14005000400009),   UINT64_C(0xd21001001c00045),
		UINT64_C(0xc0003000200024),   UINT64_C(0x40003000280004),   UINT64_C(0x40021000091102),
		UINT64_C(0x2008a20408000d00), UINT64_C(0x2000100084010040), UINT64_C(0x144080008008001),
		UINT64_C(0x50102400100026a2), UINT64_C(0x1040020008001010), UINT64_C(0x1200200028005010),
		UINT64_C(0x4280030030020898), UINT64_C(0x480081410011004),  UINT64_C(0x34000040800110a),
		UINT64_C(0x101000010c0021),   UINT64_C(0x9210800080082),    UINT64_C(0x6100002000400a7),
		UINT64_C(0xa2240800900800c0), UINT64_C(0x9220082001000801), UINT64_C(0x1040008001140030),
		UINT64_C(0x40002220040008),   UINT64_C(0x28000124008010c),  UINT64_C(0x40008404940002),
		UINT64_C(0x40040800010200),   UINT64_C(0x90000809002100),   UINT64_C(0x2800080001000201),
		UINT64_C(0x1400020001000201), UINT64_C(0x180081014018004),  UINT64_C(0x1100008000400201),
		UINT64_C(0x80004000200201),   UINT64_C(0x420800010000201),  UINT64_C(0x2841c00080200209),
		UINT64_C(0x120002401040001),  UINT64_C(0x14510000101000b),  UINT64_C(0x40080000808001),
		UINT64_C(0x834000188048001),  UINT64_C(0x4001210000800205), UINT64_C(0x4889a8007400201),
		UINT64_C(0x2080044080200062), UINT64_C(0x80004002861002),   UINT64_C(0xc00842049024),
		UINT64_C(0x8040000202020011), UINT64_C(0x400404002c0100),   UINT64_C(0x2080028202000102),
		UINT64_C(0x8100040800590224), UINT64_C(0x2040009004800010), UINT64_C(0x40045000400408),
		UINT64_C(0x2200240020802008), UINT64_C(0x4080042002200204), UINT64_C(0x4000b0000a00a2),
		UINT64_C(0xa600000810100),    UINT64_C(0x1410000d001180),   UINT64_C(0x2200101001080),
		UINT64_C(0x100020014104e120), UINT64_C(0x2407200100004810), UINT64_C(0x80144000a0845050),
		UINT64_C(0x1000200060030c18), UINT64_C(0x4004200020010102), UINT64_C(0x140600021010302)
};
#endif


	// メモリ節約の為、1次元配列にして無駄が無いようにしている。
#if defined HAVE_BMI2
	Bitboard m_controllBb_[495616];
#else
	Bitboard m_controllBb_[512000];
#endif

	int		m_rookAttackIndex[SquareNum];
	Bitboard m_rookBlockMask_[SquareNum];
	Bitboard m_controllBbToEdge_[SquareNum];



public:

#if defined FIND_MAGIC
	u64 findMagicRook(const Square sqare);
#endif // #if defined FIND_MAGIC

	// 初期化用
	Bitboard RookBlockMaskCalc(const Square square) const;
	Bitboard RookAttackCalc(const Square square, const Bitboard& occupied) const;
	void InitRookAttacks();

	// 障害物が無いときの利きの Bitboard
	// g_rookAttack, g_bishopAttack, g_lanceAttack を設定してから、この関数を呼ぶこと。
	void InitializeToEdge();

	inline Bitboard GetControllBbToEdge(const Square sq) const {
		return this->m_controllBbToEdge_[sq];
	}

	// 飛車の縦だけの利き。香車の利きを使い、index を共通化することで高速化している。
	inline Bitboard GetControllBbFile(const Bitboard* thisBitboard, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*thisBitboard).GetP(part) >> g_slideBits.m_slide[sq]) & 127;
		return g_lanceAttackBb.m_controllBb[Black][sq][index] | g_lanceAttackBb.m_controllBb[White][sq][index];
	}

	// todo: テーブル引きを検討
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
				this->m_rookAttackIndex[sq] +
					block.OccupiedToIndex(this->m_rookMagic_[sq], this->m_rookShiftBits_[sq])
			];
		}
	#endif

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern RookAttackBb g_rookAttackBb;
