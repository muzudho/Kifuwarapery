#pragma once


#include "n160_050_configBits.hpp"
#include "n160_100_bitboard.hpp"
#include "n160_110_silverAttackBb.hpp"


extern ConfigBits g_configBits;
extern SilverAttackBb g_silverAttackBb;


//────────────────────────────────────────────────────────────────────────────────
// 角
//────────────────────────────────────────────────────────────────────────────────
class BishopAttackBb {
private:
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
				block.OccupiedToIndex(g_configBits.m_bishopMagic[sq], g_configBits.m_bishopShiftBits[sq])
			];
		}
	#endif

};
