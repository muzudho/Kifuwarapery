#include "../../header/n160_board___/n160_108_slideBits.hpp"
#include "../../header/n160_board___/n160_102_FileMaskBb.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_130_lanceAttackBb.hpp"
#include "../../header/n160_board___/n160_140_goldAttackBb.hpp"
#include "../../header/n160_board___/n160_150_rookAttackBb.hpp"
#include "../../header/n160_board___/n160_190_pawnAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"


// これらは一度値を設定したら二度と変更しない。
LanceAttackBb g_lanceAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


void LanceAttackBb::Initialize()
{
	// LanceBlockMask, g_lanceAttack の値を設定する。
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			const Bitboard blockMask = this->LanceBlockMask(sq);
			//const int num1s = blockMask.popCount(); // 常に 7
			const int num1s = 7;
			assert(num1s == blockMask.PopCount());
			for (int i = 0; i < (1 << num1s); ++i) {
				Bitboard occupied = g_setMaskBb.IndexToOccupied(i, num1s, blockMask);
				this->m_controllBb[c][sq][i] = this->LanceAttackCalc(c, sq, occupied);
			}
		}
	}
}


void LanceAttackBb::InitCheckTableLance() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			this->m_lanceCheckTable_[c][sq] = this->GetControllBbToEdge(opp, sq);

			const Bitboard TRank789BB = (c == Black ?
				g_inFrontMaskBb.GetInFrontMask(Black, Rank6) :
				g_inFrontMaskBb.GetInFrontMask(White, Rank4)
			);
			Bitboard checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.Exists1Bit()) {
				const Square checkSq = checkBB.PopFirstOneFromI9();
				this->m_lanceCheckTable_[c][sq] |= this->GetControllBbToEdge(opp, checkSq);
			}
			this->m_lanceCheckTable_[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_pawnAttackBb.GetControllBb(opp, sq));
		}
	}
}


void LanceAttackBb::InitializeToEdge()
{
	for (Square sq = I9; sq < SquareNum; ++sq) {
		this->m_controllBbToEdge_[Black][sq] = this->GetControllBb(&Bitboard::CreateAllZeroBB(), Black, sq);
		this->m_controllBbToEdge_[White][sq] = this->GetControllBb(&Bitboard::CreateAllZeroBB(), White, sq);
	}
}


// square のマスにおける、障害物を調べる必要がある場所を Bitboard で返す。
// lance の前方だけを調べれば良さそうだけど、Rank8 ~ Rank2 の状態をそのまま index に使いたいので、
// 縦方向全て(端を除く)の occupied を全て調べる。
Bitboard LanceAttackBb::LanceBlockMask(const Square square) {
	return g_fileMaskBb.GetSquareFileMask(square) & ~(g_rankMaskBb.GetRankMask_rank(Rank1) | g_rankMaskBb.GetRankMask_rank(Rank9));
}

// lance の利きを返す。
// 香車の利きは常にこれを使っても良いけど、もう少し速くする為に、テーブル化する為だけに使う。
// occupied  障害物があるマスが 1 の bitboard
Bitboard LanceAttackBb::LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied) {
	return g_rookAttackBb.GetControllBb(&occupied, square) & g_inFrontMaskBb.GetInFrontMask(c, UtilSquare::ToRank(square));
}
