#include "../../header/n110_square__/n110_500_convSquare.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"


//────────────────────────────────────────────────────────────────────────────────
// マスク
//────────────────────────────────────────────────────────────────────────────────
const SetMaskBb g_setMaskBb;


void SetMaskBb::ClearBit(Bitboard* thisBitboard, const Square sq) const
{
	(*thisBitboard).AndEqualNot(this->m_setMaskBB_[sq]);
}

void SetMaskBb::XorBit(Bitboard* thisBitboard, const Square sq) const
{
	(*thisBitboard) ^= this->m_setMaskBB_[sq];
}

void SetMaskBb::XorBit(Bitboard* thisBitboard, const Square sq1, const Square sq2) const
{
	(*thisBitboard) ^= (this->m_setMaskBB_[sq1] | this->m_setMaskBB_[sq2]);
}

bool SetMaskBb::IsSet(const Bitboard* thisBitboard, const Square sq) const
{
	assert(ConvSquare::ContainsOf(sq));
	return thisBitboard->AndIsNot0(g_setMaskBb.m_setMaskBB_[sq]);
}

void SetMaskBb::AddBit(Bitboard* thisBitboard, const Square sq) const
{
	*thisBitboard |= g_setMaskBb.m_setMaskBB_[sq];
}

// index, bits の情報を元にして、occupied の 1 のbit を いくつか 0 にする。
// index の値を, occupied の 1のbit の位置に変換する。
// index   [0, 1<<bits) の範囲のindex
// bits    bit size
// blockMask   利きのあるマスが 1 のbitboard
// result  occupied
Bitboard SetMaskBb::IndexToOccupied(const int index, const int bits, const Bitboard& blockMask) const {
	Bitboard tmpBlockMask = blockMask;
	Bitboard result = Bitboard::CreateAllZeroBB();
	for (int i = 0; i < bits; ++i) {
		const Square sq = tmpBlockMask.PopFirstOneFromI9();
		if (index & (1 << i))
			this->AddBit(&result, sq);
	}
	return result;
}
