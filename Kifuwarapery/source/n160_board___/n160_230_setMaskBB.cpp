#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"


//────────────────────────────────────────────────────────────────────────────────
// マスク
//────────────────────────────────────────────────────────────────────────────────
SetMaskBb g_setMaskBb;


void SetMaskBb::ClearBit(Bitboard* thisBitboard, const Square sq)
{
	(*thisBitboard).AndEqualNot(this->m_setMaskBB[sq]);
}

void SetMaskBb::XorBit(Bitboard* thisBitboard, const Square sq)
{
	(*thisBitboard) ^= this->m_setMaskBB[sq];
}

void SetMaskBb::XorBit(Bitboard* thisBitboard, const Square sq1, const Square sq2)
{
	(*thisBitboard) ^= (this->m_setMaskBB[sq1] | this->m_setMaskBB[sq2]);
}

bool SetMaskBb::IsSet(const Bitboard* thisBitboard, const Square sq) const
{
	assert(UtilSquare::ContainsOf(sq));
	return thisBitboard->AndIsNot0(g_setMaskBb.m_setMaskBB[sq]);
}

void SetMaskBb::SetBit(Bitboard* thisBitboard, const Square sq)
{
	*thisBitboard |= g_setMaskBb.m_setMaskBB[sq];
}

// index, bits の情報を元にして、occupied の 1 のbit を いくつか 0 にする。
// index の値を, occupied の 1のbit の位置に変換する。
// index   [0, 1<<bits) の範囲のindex
// bits    bit size
// blockMask   利きのあるマスが 1 のbitboard
// result  occupied
Bitboard SetMaskBb::IndexToOccupied(const int index, const int bits, const Bitboard& blockMask) {
	Bitboard tmpBlockMask = blockMask;
	Bitboard result = Bitboard::AllZeroBB();
	for (int i = 0; i < bits; ++i) {
		const Square sq = tmpBlockMask.FirstOneFromI9();
		if (index & (1 << i))
			this->SetBit(&result, sq);
	}
	return result;
}
