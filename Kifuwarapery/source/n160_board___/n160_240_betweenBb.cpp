#include "../../header/n160_board___/n160_250_squareRelation.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_240_betweenBb.hpp"


extern SquareRelation g_squareRelation;

BetweenBb g_betweenBb;

void BetweenBb::Initialize() {
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			g_betweenBb.m_betweenBB_[sq1][sq2] = Bitboard::CreateAllZeroBB();
			if (sq1 == sq2) continue;
			const Direction direc = g_squareRelation.GetSquareRelation(sq1, sq2);
			if (direc & DirecCross)
				g_betweenBb.m_betweenBB_[sq1][sq2] = g_rookAttackBb.GetControllBb(&g_setMaskBb.GetSetMaskBb(sq2), sq1) & g_rookAttackBb.GetControllBb(&g_setMaskBb.GetSetMaskBb(sq1), sq2);
			else if (direc & DirecDiag)
				g_betweenBb.m_betweenBB_[sq1][sq2] = g_bishopAttackBb.BishopAttack(&g_setMaskBb.GetSetMaskBb(sq2), sq1) & g_bishopAttackBb.BishopAttack(&g_setMaskBb.GetSetMaskBb(sq1), sq2);
		}
	}
}
