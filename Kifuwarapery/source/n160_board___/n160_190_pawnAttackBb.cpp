#include "../../header/n160_board___/n160_110_silverAttackBb.hpp"
#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"
#include "../../header/n160_board___/n160_190_pawnAttackBb.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"


PawnAttackBb g_pawnAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


//────────────────────────────────────────────────────────────────────────────────
// 歩
//────────────────────────────────────────────────────────────────────────────────
void PawnAttackBb::Initialize()
{
	for (Color c = Black; c < g_COLOR_NUM; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_pawnAttackBb.m_controllBb_[c][sq] =
				g_silverAttackBb.GetControllBb(c, sq) ^ g_bishopAttackBb.BishopAttack(Bitboard::CreateAllOneBB(), sq);
}
