#include "../../header/n160_board___/n160_120_bishopAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_102_FileMaskBb.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"


BishopAttackBb g_bishopAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


#if defined FIND_MAGIC
// square の位置の rook, bishop それぞれのMagic Bitboard に使用するマジックナンバーを見つける。
// isBishop  : true なら bishop, false なら rook のマジックナンバーを見つける。
u64 BishopAttackBb::findMagicBishop(const Square square) {
	Bitboard occupied[1 << 14];
	Bitboard attack[1 << 14];
	Bitboard attackUsed[1 << 14];
	Bitboard mask = BishopBlockMaskCalc(square);
	int num1s = g_bishopBlockBits[square];

	// n bit の全ての数字 (利きのあるマスの全ての 0 or 1 の組み合わせ)
	for (int i = 0; i < (1 << num1s); ++i) {
		occupied[i] = g_setMaskBb.IndexToOccupied(i, num1s, mask);
		attack[i] = BishopAttackCalc(square, occupied[i]);
	}

	for (u64 k = 0; k < UINT64_C(100000000); ++k) {
		const u64 magic = g_mt64bit.GetRandomFewBits();
		bool fail = false;

		// これは無くても良いけど、少しマジックナンバーが見つかるのが早くなるはず。
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


// square のマスにおける、障害物を調べる必要がある場所を調べて Bitboard で返す。
Bitboard BishopAttackBb::BishopBlockMaskCalc(const Square square) const {
	const Rank rank = ConvSquare::TO_RANK10(square);
	const File file = ConvSquare::TO_FILE10(square);
	Bitboard result = Bitboard::CreateAllZeroBB();
	for (Square sq = I9; sq < SquareNum; ++sq) {
		const Rank r = ConvSquare::TO_RANK10(sq);
		const File f = ConvSquare::TO_FILE10(sq);
		if (abs(rank - r) == abs(file - f))
			g_setMaskBb.AddBit(&result, sq);
	}
	result &= ~(
		g_rankMaskBb.GetRankMask<Rank1>() |
		g_rankMaskBb.GetRankMask<Rank9>() |
		g_fileMaskBb.GetFileMask(FileA) |
		g_fileMaskBb.GetFileMask(FileI)
	);
	g_setMaskBb.ClearBit(&result, square);

	return result;
}

// Rook or Bishop の利きの範囲を調べて bitboard で返す。
// occupied  障害物があるマスが 1 の bitboard
Bitboard BishopAttackBb::BishopAttackCalc(const Square square, const Bitboard& occupied) const {
	const SquareDelta deltaArray[2][4] = { { DeltaN, DeltaS, DeltaE, DeltaW },{ DeltaNE, DeltaSE, DeltaSW, DeltaNW } };
	Bitboard result = Bitboard::CreateAllZeroBB();
	for (SquareDelta delta : deltaArray[true/*isBishop*/]) {
		for (Square sq = square + delta;
		ConvSquare::CONTAINS_OF10(sq) && abs(ConvSquare::TO_RANK10(sq - delta) - ConvSquare::TO_RANK10(sq)) <= 1;
			sq += delta)
		{
			g_setMaskBb.AddBit(&result, sq);
			if (g_setMaskBb.IsSet(&occupied, sq))
				break;
		}
	}

	return result;
}


void BishopAttackBb::InitBishopAttacks()
{
	// 角か、飛車か
	auto* attacks = g_bishopAttackBb.m_controllBb_;
	auto* attackIndex = g_bishopAttackBb.m_controllBbIndex_;
	auto* blockMask = g_bishopAttackBb.m_bishopBlockMask_;
	auto* shift = g_bishopAttackBb.m_bishopShiftBits;
#if defined HAVE_BMI2
#else
	auto* magic = g_bishopAttackBb.m_bishopMagic;
#endif

	// 各マスについて
	int index = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		blockMask[sq] = this->BishopBlockMaskCalc(sq);
		attackIndex[sq] = index;

		const int num1s = g_bishopAttackBb.m_bishopBlockBits[sq];
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
		g_bishopAttackBb.m_controllBbToEdge_[sq] = g_bishopAttackBb.BishopAttack(Bitboard::CreateAllZeroBB(), sq);
	}
}
