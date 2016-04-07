#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_500_bitboardMask.hpp"


extern SetMaskBb g_setMaskBb;


// �����͈�x�l��ݒ肵�����x�ƕύX���Ȃ��B
RookAttackBb g_rookAttackBb;//�{����const �ɂ��������A�������킩��Ȃ��� C2373�G���[�ɂȂ�񂾂���


#if defined FIND_MAGIC
// square �̈ʒu�� rook, bishop ���ꂼ���Magic Bitboard �Ɏg�p����}�W�b�N�i���o�[��������B
// isBishop  : true �Ȃ� bishop, false �Ȃ� rook �̃}�W�b�N�i���o�[��������B
u64 RookAttackBb::findMagicRook(const Square square) {
	Bitboard occupied[1 << 14];
	Bitboard attack[1 << 14];
	Bitboard attackUsed[1 << 14];
	Bitboard mask = this->RookBlockMaskCalc(square);
	int num1s = g_rookBlockBits[square];

	// n bit �̑S�Ă̐��� (�����̂���}�X�̑S�Ă� 0 or 1 �̑g�ݍ��킹)
	for (int i = 0; i < (1 << num1s); ++i) {
		occupied[i] = g_setMaskBb.IndexToOccupied(i, num1s, mask);
		attack[i] = RookAttackCalc(square, occupied[i]);
	}

	for (u64 k = 0; k < UINT64_C(100000000); ++k) {
		const u64 magic = g_mt64bit.GetRandomFewBits();
		bool fail = false;

		// ����͖����Ă��ǂ����ǁA�����}�W�b�N�i���o�[��������̂������Ȃ�͂��B
		if (count1s((mask.MergeP() * magic) & UINT64_C(0xfff0000000000000)) < 6)
			continue;

		std::fill(std::begin(attackUsed), std::IsEnd(attackUsed), AllZeroBB());

		for (int i = 0; !fail && i < (1 << num1s); ++i) {
			const int shiftBits = g_rookShiftBits[square];
			const u64 index = OccupiedToIndex(occupied[i], magic, shiftBits);
			if (attackUsed[index] == AllZeroBB())
				attackUsed[index] = attack[i];
			else if (attackUsed[index] != attack[i])
				fail = true;
		}
		if (!fail)
			return magic;
	}

	std::cout << "/***Failed***/\t";
	return 0;
}
#endif // #if defined FIND_MAGIC


// square �̃}�X�ɂ�����A��Q���𒲂ׂ�K�v������ꏊ�𒲂ׂ� Bitboard �ŕԂ��B
Bitboard RookAttackBb::RookBlockMaskCalc(const Square square) {
	Bitboard result = BitboardMask::GetSquareFileMask(square) ^ BitboardMask::GetSquareRankMask(square);
	if (UtilSquare::ToFile(square) != FileA) { result &= ~BitboardMask::GetFileMask<FileA>(); }
	if (UtilSquare::ToFile(square) != FileI) { result &= ~BitboardMask::GetFileMask<FileI>(); }
	if (UtilSquare::ToRank(square) != Rank1) { result &= ~BitboardMask::GetRankMask<Rank1>(); }
	if (UtilSquare::ToRank(square) != Rank9) { result &= ~BitboardMask::GetRankMask<Rank9>(); }
	return result;
}


// Rook or Bishop �̗����͈̔͂𒲂ׂ� bitboard �ŕԂ��B
// occupied  ��Q��������}�X�� 1 �� bitboard
Bitboard RookAttackBb::RookAttackCalc(const Square square, const Bitboard& occupied) {
	const SquareDelta deltaArray[2][4] = { { DeltaN, DeltaS, DeltaE, DeltaW },{ DeltaNE, DeltaSE, DeltaSW, DeltaNW } };
	Bitboard result = Bitboard::AllZeroBB();
	for (SquareDelta delta : deltaArray[false/*isBishop*/]) {
		for (Square sq = square + delta;
		UtilSquare::ContainsOf(sq) && abs(UtilSquare::ToRank(sq - delta) - UtilSquare::ToRank(sq)) <= 1;
			sq += delta)
		{
			g_setMaskBb.SetBit(&result, sq);
			if (g_setMaskBb.IsSet(&occupied, sq))
				break;
		}
	}

	return result;
}


void RookAttackBb::InitRookAttacks()
{
	// �p���A��Ԃ�
	auto* attacks = g_rookAttackBb.m_controllBb;
	auto* attackIndex = ConfigBits::m_rookAttackIndex;
	auto* blockMask = g_rookAttackBb.m_rookBlockMask;
	auto* shift = ConfigBits::m_rookShiftBits;
#if defined HAVE_BMI2
#else
	auto* magic = ConfigBits::m_rookMagic;
#endif

	// �e�}�X�ɂ���
	int index = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		blockMask[sq] = this->RookBlockMaskCalc(sq);
		attackIndex[sq] = index;

		const int num1s = ConfigBits::m_rookBlockBits[sq];
		for (int i = 0; i < (1 << num1s); ++i)
		{
			const Bitboard occupied = g_setMaskBb.IndexToOccupied(i, num1s, blockMask[sq]);
#if defined HAVE_BMI2
			attacks[index + (occupied & blockMask[sq]).OccupiedToIndex(blockMask[sq])] = this->RookAttackCalc(sq, occupied);
#else
			attacks[index + occupied.OccupiedToIndex(magic[sq], shift[sq])] =
				this->RookAttackCalc(sq, occupied);
#endif
		}
		index += 1 << (64 - shift[sq]);
	}
}


void RookAttackBb::InitializeToEdge()
{
	for (Square sq = I9; sq < SquareNum; ++sq) {
		g_rookAttackBb.m_controllBbToEdge[sq] = g_rookAttackBb.GetControllBb(&Bitboard::AllZeroBB(), sq);
	}
}