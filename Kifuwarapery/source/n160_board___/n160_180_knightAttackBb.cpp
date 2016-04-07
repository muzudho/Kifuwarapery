#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"


extern const InFrontMaskBb g_inFrontMaskBb;


KnightAttackBb g_knightAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


//────────────────────────────────────────────────────────────────────────────────
// 桂
//────────────────────────────────────────────────────────────────────────────────
void KnightAttackBb::Initialize()
{
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_knightAttackBb.m_controllBb_[c][sq] = Bitboard::CreateAllZeroBB();
			const Bitboard bb = g_pawnAttackBb.GetControllBb(c, sq);
			if (bb.Exists1Bit())
				g_knightAttackBb.m_controllBb_[c][sq] =
					g_bishopAttackBb.BishopStepAttacks(
						bb.GetFirstOneFromI9()) &
						g_inFrontMaskBb.GetInFrontMask(c, UtilSquare::ToRank(sq)
					);
		}
	}
}

void KnightAttackBb::InitCheckTableKnight() {
	for (Color color = Black; color < ColorNum; ++color) {
		const Color opponent = UtilColor::OppositeColor(color);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			this->m_knightCheckTable_[color][sq] = Bitboard::CreateAllZeroBB();

			// 相手の桂馬の利き
			Bitboard checkBB = this->GetControllBb(opponent, sq);
			while (checkBB.Exists1Bit()) {
				const Square checkSq = checkBB.PopFirstOneFromI9();
				this->m_knightCheckTable_[color][sq] |= this->GetControllBb(opponent, checkSq);
			}

			const Bitboard TRank789BB = (color == Black ? g_inFrontMaskBb.GetInFrontMask<Black, Rank6>() : g_inFrontMaskBb.GetInFrontMask<White, Rank4>());
			checkBB = g_goldAttackBb.GetControllBb(opponent, sq) & TRank789BB;
			while (checkBB.Exists1Bit()) {
				const Square checkSq = checkBB.PopFirstOneFromI9();
				this->m_knightCheckTable_[color][sq] |= this->GetControllBb(opponent, checkSq);
			}
		}
	}
}
