#include "../../header/n110_square__/n110_400_squareRelation.hpp"
#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"
#include "../../header/n160_board___/n160_150_rookAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_240_betweenBb.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"


BetweenBb g_betweenBb;


void BetweenBb::Initialize() {
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			g_betweenBb.m_betweenBB_[sq1][sq2] = Bitboard::CreateAllZeroBB();
			if (sq1 == sq2) continue;
			const Direction direc = g_squareRelation.GetSquareRelation(sq1, sq2);
			if (direc & N02_DirecCross)
				g_betweenBb.m_betweenBB_[sq1][sq2] = g_rookAttackBb.GetControllBb(g_setMaskBb.GetSetMaskBb(sq2), sq1) & g_rookAttackBb.GetControllBb(g_setMaskBb.GetSetMaskBb(sq1), sq2);
			else if (direc & N04_DirecDiag)
				g_betweenBb.m_betweenBB_[sq1][sq2] = g_bishopAttackBb.BishopAttack(g_setMaskBb.GetSetMaskBb(sq2), sq1) & g_bishopAttackBb.BishopAttack(g_setMaskBb.GetSetMaskBb(sq1), sq2);
		}
	}
}
