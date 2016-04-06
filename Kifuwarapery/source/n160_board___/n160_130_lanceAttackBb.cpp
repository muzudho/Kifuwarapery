#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"


// �����͈�x�l��ݒ肵�����x�ƕύX���Ȃ��B
LanceAttackBb g_lanceAttackBb;//�{����const �ɂ��������A�������킩��Ȃ��� C2373�G���[�ɂȂ�񂾂���


void LanceAttackBb::Initialize()
{
	// LanceBlockMask, g_lanceAttack �̒l��ݒ肷��B
	for (Color c = Black; c < ColorNum; ++c) {
		for (Square sq = I9; sq < SquareNum; ++sq) {
			const Bitboard blockMask = this->LanceBlockMask(sq);
			//const int num1s = blockMask.popCount(); // ��� 7
			const int num1s = 7;
			assert(num1s == blockMask.PopCount());
			for (int i = 0; i < (1 << num1s); ++i) {
				Bitboard occupied = g_setMaskBb.IndexToOccupied(i, num1s, blockMask);
				g_lanceAttackBb.m_controllBb[c][sq][i] = this->LanceAttackCalc(c, sq, occupied);
			}
		}
	}
}


void LanceAttackBb::InitCheckTableLance() {
	for (Color c = Black; c < ColorNum; ++c) {
		const Color opp = UtilColor::OppositeColor(c);
		for (Square sq = I9; sq < SquareNum; ++sq) {
			g_lanceAttackBb.m_lanceCheckTable[c][sq] = g_lanceAttackBb.GetControllBbToEdge(opp, sq);

			const Bitboard TRank789BB = (c == Black ? BitboardMask::GetInFrontMask<Black, Rank6>() : BitboardMask::GetInFrontMask<White, Rank4>());
			Bitboard checkBB = g_goldAttackBb.GetControllBb(opp, sq) & TRank789BB;
			while (checkBB.IsNot0()) {
				const Square checkSq = checkBB.FirstOneFromI9();
				g_lanceAttackBb.m_lanceCheckTable[c][sq] |= g_lanceAttackBb.GetControllBbToEdge(opp, checkSq);
			}
			g_lanceAttackBb.m_lanceCheckTable[c][sq].AndEqualNot(g_setMaskBb.GetSetMaskBb(sq) | g_pawnAttackBb.GetControllBb(opp, sq));
		}
	}
}


void LanceAttackBb::InitializeToEdge()
{
	for (Square sq = I9; sq < SquareNum; ++sq) {
		g_lanceAttackBb.m_controllBbToEdge[Black][sq] = g_lanceAttackBb.GetControllBb(&Bitboard::AllZeroBB(), Black, sq);
		g_lanceAttackBb.m_controllBbToEdge[White][sq] = g_lanceAttackBb.GetControllBb(&Bitboard::AllZeroBB(), White, sq);
	}
}


// square �̃}�X�ɂ�����A��Q���𒲂ׂ�K�v������ꏊ�� Bitboard �ŕԂ��B
// lance �̑O�������𒲂ׂ�Ηǂ����������ǁARank8 ~ Rank2 �̏�Ԃ����̂܂� index �Ɏg�������̂ŁA
// �c�����S��(�[������)�� occupied ��S�Ē��ׂ�B
Bitboard LanceAttackBb::LanceBlockMask(const Square square) {
	return BitboardMask::GetSquareFileMask(square) & ~(BitboardMask::GetRankMask<Rank1>() | BitboardMask::GetRankMask<Rank9>());
}

// lance �̗�����Ԃ��B
// ���Ԃ̗����͏�ɂ�����g���Ă��ǂ����ǁA����������������ׂɁA�e�[�u��������ׂ����Ɏg���B
// occupied  ��Q��������}�X�� 1 �� bitboard
Bitboard LanceAttackBb::LanceAttackCalc(const Color c, const Square square, const Bitboard& occupied) {
	return g_rookAttackBb.GetControllBb(&occupied, square) & BitboardMask::GetInFrontMask(c, UtilSquare::ToRank(square));
}
