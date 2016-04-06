#include "../../header/n160_board___/n160_400_printBb.hpp"


PawnAttackBb g_pawnAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


/*
//────────────────────────────────────────────────────────────────────────────────
// 歩
//────────────────────────────────────────────────────────────────────────────────
Bitboard UtilBitboard::m_pawnAttack[ColorNum][SquareNum];
*/

void PawnAttackBb::Initialize()
{
	for (Color c = Black; c < ColorNum; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_pawnAttackBb.m_controllBb[c][sq] = g_silverAttackBb.GetControllBb(c, sq) ^ g_bishopAttackBb.BishopAttack(&Bitboard::AllOneBB(), sq);
}
