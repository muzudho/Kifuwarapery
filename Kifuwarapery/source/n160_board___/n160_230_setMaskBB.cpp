#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �}�X�N
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
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

// index, bits �̏������ɂ��āAoccupied �� 1 ��bit �� ������ 0 �ɂ���B
// index �̒l��, occupied �� 1��bit �̈ʒu�ɕϊ�����B
// index   [0, 1<<bits) �͈̔͂�index
// bits    bit size
// blockMask   �����̂���}�X�� 1 ��bitboard
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
