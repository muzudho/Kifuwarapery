#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"


GoldAttackBb g_goldAttackBb;//�{����const �ɂ��������A�������킩��Ȃ��� C2373�G���[�ɂȂ�񂾂���


void GoldAttackBb::Initialize()
{
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_goldAttackBb.m_controllBb_[c][sq] =
			(
				g_kingAttackBb.GetControllBb(sq) &
				BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(sq))
			) |
			g_rookAttackBb.GetControllBb(&Bitboard::AllOneBB(), sq);
}
