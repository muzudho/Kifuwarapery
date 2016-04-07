#include "../../header/n160_board___/n160_050_configBits.hpp"
#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_102_FileMaskBb.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"

extern ConfigBits g_configBits;
extern SetMaskBb g_setMaskBb;
//extern const FileMaskBb g_fileMaskBb;


BishopAttackBb g_bishopAttackBb;//�{����const �ɂ��������A�������킩��Ȃ��� C2373�G���[�ɂȂ�񂾂���


#if defined FIND_MAGIC
// square �̈ʒu�� rook, bishop ���ꂼ���Magic Bitboard �Ɏg�p����}�W�b�N�i���o�[��������B
// isBishop  : true �Ȃ� bishop, false �Ȃ� rook �̃}�W�b�N�i���o�[��������B
u64 BishopAttackBb::findMagicBishop(const Square square) {
	Bitboard occupied[1 << 14];
	Bitboard attack[1 << 14];
	Bitboard attackUsed[1 << 14];
	Bitboard mask = BishopBlockMaskCalc(square);
	int num1s = g_bishopBlockBits[square];

	// n bit �̑S�Ă̐��� (�����̂���}�X�̑S�Ă� 0 or 1 �̑g�ݍ��킹)
	for (int i = 0; i < (1 << num1s); ++i) {
		occupied[i] = g_setMaskBb.IndexToOccupied(i, num1s, mask);
		attack[i] = BishopAttackCalc(square, occupied[i]);
	}

	for (u64 k = 0; k < UINT64_C(100000000); ++k) {
		const u64 magic = g_mt64bit.GetRandomFewBits();
		bool fail = false;

		// ����͖����Ă��ǂ����ǁA�����}�W�b�N�i���o�[��������̂������Ȃ�͂��B
		if (count1s((mask.MergeP() * magic) & UINT64_C(0xfff0000000000000)) < 6)
			continue;

		std::fill(std::begin(attackUsed), std::IsEnd(attackUsed), CreateAllZeroBB());

		for (int i = 0; !fail && i < (1 << num1s); ++i) {
			const int shiftBits = g_bishopShiftBits[square];
			const u64 index = OccupiedToIndex(occupied[i], magic, shiftBits);
			if (attackUsed[index] == CreateAllZeroBB())
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
Bitboard BishopAttackBb::BishopBlockMaskCalc(const Square square) const {
	const Rank rank = UtilSquare::ToRank(square);
	const File file = UtilSquare::ToFile(square);
	Bitboard result = Bitboard::CreateAllZeroBB();
	for (Square sq = I9; sq < SquareNum; ++sq) {
		const Rank r = UtilSquare::ToRank(sq);
		const File f = UtilSquare::ToFile(sq);
		if (abs(rank - r) == abs(file - f))
			g_setMaskBb.SetBit(&result, sq);
	}
	result &= ~(
		g_rankMaskBb.GetRankMask<Rank1>() |
		g_rankMaskBb.GetRankMask<Rank9>() |
		g_fileMaskBb.GetFileMask<FileA>() |
		g_fileMaskBb.GetFileMask<FileI>()
	);
	g_setMaskBb.ClearBit(&result, square);

	return result;
}

// Rook or Bishop �̗����͈̔͂𒲂ׂ� bitboard �ŕԂ��B
// occupied  ��Q��������}�X�� 1 �� bitboard
Bitboard BishopAttackBb::BishopAttackCalc(const Square square, const Bitboard& occupied) const {
	const SquareDelta deltaArray[2][4] = { { DeltaN, DeltaS, DeltaE, DeltaW },{ DeltaNE, DeltaSE, DeltaSW, DeltaNW } };
	Bitboard result = Bitboard::CreateAllZeroBB();
	for (SquareDelta delta : deltaArray[true/*isBishop*/]) {
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


void BishopAttackBb::InitBishopAttacks()
{
	// �p���A��Ԃ�
	auto* attacks = g_bishopAttackBb.m_controllBb_;
	auto* attackIndex = g_bishopAttackBb.m_controllBbIndex_;
	auto* blockMask = g_bishopAttackBb.m_bishopBlockMask_;
	auto* shift = g_configBits.m_bishopShiftBits;
#if defined HAVE_BMI2
#else
	auto* magic = g_configBits.m_bishopMagic;
#endif

	// �e�}�X�ɂ���
	int index = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		blockMask[sq] = this->BishopBlockMaskCalc(sq);
		attackIndex[sq] = index;

		const int num1s = g_configBits.m_bishopBlockBits[sq];
		for (int i = 0; i < (1 << num1s); ++i)
		{
			const Bitboard occupied = g_setMaskBb.IndexToOccupied(i, num1s, blockMask[sq]);
#if defined HAVE_BMI2
			attacks[index + (occupied & blockMask[sq]).OccupiedToIndex(blockMask[sq])] = this->BishopAttackCalc(sq, occupied);
#else
			attacks[index + occupied.OccupiedToIndex(magic[sq], shift[sq])] =
				this->BishopAttackCalc(sq, occupied);
#endif
		}
		index += 1 << (64 - shift[sq]);
	}
}



void BishopAttackBb::InitializeToEdge()
{
	for (Square sq = I9; sq < SquareNum; ++sq) {
		g_bishopAttackBb.m_controllBbToEdge_[sq] = g_bishopAttackBb.BishopAttack(&Bitboard::CreateAllZeroBB(), sq);
	}
}
