#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_110_silverAttackBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"

SilverAttackBb g_silverAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆

void SilverAttackBb::Initialize()
{
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_silverAttackBb.m_controllBb[c][sq] = (
				g_kingAttackBb.GetControllBb(sq) &
				BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(sq))
			) |
			g_bishopAttackBb.BishopAttack(&Bitboard::AllOneBB(), sq);
}
