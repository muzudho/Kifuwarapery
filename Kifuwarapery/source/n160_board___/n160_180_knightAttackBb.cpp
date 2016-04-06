#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"


KnightAttackBb g_knightAttackBb;//�{����const �ɂ��������A�������킩��Ȃ��� C2373�G���[�ɂȂ�񂾂���


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// �j
//����������������������������������������������������������������������������������������������������������������������������������������������������������������
void KnightAttackBb::Initialize()
{
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_knightAttackBb.m_controllBb[c][sq] = Bitboard::AllZeroBB();
			const Bitboard bb = g_pawnAttackBb.GetControllBb(c, sq);
			if (bb.IsNot0())
				g_knightAttackBb.m_controllBb[c][sq] =
					g_bishopAttackBb.BishopStepAttacks(
						bb.ConstFirstOneFromI9()) &
						BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(sq)
					);
		}
	}
}
