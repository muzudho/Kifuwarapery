#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_140_goldAttackBb.hpp"
#include "../../header/n160_board___/n160_150_rookAttackBb.hpp"
#include "../../header/n160_board___/n160_160_kingAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"


GoldAttackBb g_goldAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


void GoldAttackBb::Initialize()
{
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_goldAttackBb.m_controllBb_[c][sq] =
			(
				g_kingAttackBb.GetControllBb(sq) &
				g_inFrontMaskBb.GetInFrontMask(c, ConvSquare::TO_RANK10(sq))
			) |
			g_rookAttackBb.GetControllBb(Bitboard::CreateAllOneBB(), sq);
}

void GoldAttackBb::InitCheckTableGold() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_goldAttackBb.m_goldCheckTable_[c][sq] = Bitboard::CreateAllZeroBB();
			Bitboard checkBB = g_goldAttackBb.GetControllBb(opp, sq);
			while (checkBB.Exists1Bit()) {
				const Square checkSq = checkBB.PopFirstOneFromI9();
				g_goldAttackBb.m_goldCheckTable_[c][sq] |= g_goldAttackBb.GetControllBb(opp, checkSq);
			}
			g_goldAttackBb.m_goldCheckTable_[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_goldAttackBb.GetControllBb(opp, sq));
		}
	}
}
