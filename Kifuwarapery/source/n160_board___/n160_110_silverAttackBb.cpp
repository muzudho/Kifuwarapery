#include "../../header/n110_square__/n110_500_convSquare.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_110_silverAttackBb.hpp"
#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"	// TODO:これを消すのが難しい
#include "../../header/n160_board___/n160_140_goldAttackBb.hpp"		// TODO:これを消すのが難しい
#include "../../header/n160_board___/n160_160_kingAttackBb.hpp"		// TODO:これを消すのが難しい
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"		// TODO:これを消すのが難しい


SilverAttackBb g_silverAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


void SilverAttackBb::Initialize()
{
	for (Color c = Black; c < g_COLOR_NUM; ++c)
		for (Square sq = I9; sq < SquareNum; ++sq)
			g_silverAttackBb.m_controllBb_[c][sq] = (
				g_kingAttackBb.GetControllBb(sq) &
				g_inFrontMaskBb.GetInFrontMask(c, ConvSquare::TO_RANK10(sq))
			) |
			g_bishopAttackBb.BishopAttack(Bitboard::CreateAllOneBB(), sq);
}

void SilverAttackBb::InitCheckTableSilver() {
	for (Color c = Black; c < g_COLOR_NUM; ++c) {
		const Color opp = ConvColor::OPPOSITE_COLOR10b(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			this->m_silverCheckTable_[c][sq] = Bitboard::CreateAllZeroBB();

			Bitboard checkBB = this->GetControllBb(opp, sq);
			while (checkBB.Exists1Bit()) {
				const Square checkSq = checkBB.PopFirstOneFromI9();
				this->m_silverCheckTable_[c][sq] |= this->GetControllBb(opp, checkSq);
			}
			const Bitboard TRank789BB = (c == Black ? g_inFrontMaskBb.GetInFrontMask(Black, Rank6) : g_inFrontMaskBb.GetInFrontMask(White, Rank4));
			checkBB = g_goldAttackBb.GetControllBb(opp, sq);
			while (checkBB.Exists1Bit()) {
				const Square checkSq = checkBB.PopFirstOneFromI9();
				// 移動元が敵陣である位置なら、金に成って王手出来る。
				this->m_silverCheckTable_[c][sq] |= (this->GetControllBb(opp, checkSq) & TRank789BB);
			}

			const Bitboard TRank6BB = (c == Black ? g_rankMaskBb.GetRankMask<Rank6>() : g_rankMaskBb.GetRankMask<Rank4>());
			// 移動先が3段目で、4段目に移動したときも、成ることが出来る。
			checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.Exists1Bit()) {
				const Square checkSq = checkBB.PopFirstOneFromI9();
				this->m_silverCheckTable_[c][sq] |= (this->GetControllBb(opp, checkSq) & TRank6BB);
			}
			this->m_silverCheckTable_[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | this->GetControllBb(opp, sq));
		}
	}
}
