#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"


KnightAttackBb g_knightAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


//────────────────────────────────────────────────────────────────────────────────
// 桂
//────────────────────────────────────────────────────────────────────────────────
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

void KnightAttackBb::InitCheckTableKnight() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_knightAttackBb.m_knightCheckTable[c][sq] = Bitboard::AllZeroBB();

			Bitboard checkBB = g_knightAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_knightAttackBb.m_knightCheckTable[c][sq] |= g_knightAttackBb.GetControllBb(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_knightAttackBb.m_knightCheckTable[c][sq] |= g_knightAttackBb.GetControllBb(opp, checkSq);
			}
		}
	}
}
