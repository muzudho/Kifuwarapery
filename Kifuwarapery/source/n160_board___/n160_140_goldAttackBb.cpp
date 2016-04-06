#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"


GoldAttackBb g_goldAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


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

void GoldAttackBb::InitCheckTableGold() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_goldAttackBb.m_goldCheckTable[c][sq] = Bitboard::AllZeroBB();
			Bitboard checkBB = g_goldAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_goldAttackBb.m_goldCheckTable[c][sq] |= g_goldAttackBb.GetControllBb(opp, checkSq);
			}
			g_goldAttackBb.m_goldCheckTable[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_goldAttackBb.GetControllBb(opp, sq));
		}
	}
}
