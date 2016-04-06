#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_110_silverAttackBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"

SilverAttackBb g_silverAttackBb;//�{����const �ɂ��������A�������킩��Ȃ��� C2373�G���[�ɂȂ�񂾂���

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

void SilverAttackBb::InitCheckTableSilver() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_silverAttackBb.m_silverCheckTable[c][sq] = Bitboard::AllZeroBB();

			Bitboard checkBB = g_silverAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_silverAttackBb.m_silverCheckTable[c][sq] |= g_silverAttackBb.GetControllBb(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			checkBB = g_goldAttackBb.GetControllBb(opp, sq);
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				// �ړ������G�w�ł���ʒu�Ȃ�A���ɐ����ĉ���o����B
				g_silverAttackBb.m_silverCheckTable[c][sq] |= (g_silverAttackBb.GetControllBb(opp, checkSq) & TRank789BB);
			}

			const Bitboard TRank6BB = (c == Black ? BitboardMask::GetRankMask<Rank6>() : BitboardMask::GetRankMask<Rank4>());
			// �ړ��悪3�i�ڂŁA4�i�ڂɈړ������Ƃ����A���邱�Ƃ��o����B
			checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_silverAttackBb.m_silverCheckTable[c][sq] |= (g_silverAttackBb.GetControllBb(opp, checkSq) & TRank6BB);
			}
			g_silverAttackBb.m_silverCheckTable[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_silverAttackBb.GetControllBb(opp, sq));
		}
	}
}
