#include "../../header/n160_board___/n160_102_FileMaskBb.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_108_slideBits.hpp"
#include "../../header/n160_board___/n160_150_rookAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"


// これらは一度値を設定したら二度と変更しない。
RookAttackBb g_rookAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆


#if defined FIND_MAGIC
// square の位置の rook, bishop それぞれのMagic Bitboard に使用するマジックナンバーを見つける。
// isBishop  : true なら bishop, false なら rook のマジックナンバーを見つける。
u64 RookAttackBb::findMagicRook(const Square square) {
	Bitboard occupied[1 << 14];
	Bitboard attack[1 << 14];
	Bitboard attackUsed[1 << 14];
	Bitboard mask = this->RookBlockMaskCalc(square);
	int num1s = g_rookBlockBits[square];

	// n bit の全ての数字 (利きのあるマスの全ての 0 or 1 の組み合わせ)
	for (int i = 0; i < (1 << num1s); ++i) {
		occupied[i] = g_setMaskBb.IndexToOccupied(i, num1s, mask);
		attack[i] = RookAttackCalc(square, occupied[i]);
	}

	for (u64 k = 0; k < UINT64_C(100000000); ++k) {
		const u64 magic = g_mt64bit.GetRandomFewBits();
		bool fail = false;

		// これは無くても良いけど、少しマジックナンバーが見つかるのが早くなるはず。
		if (count1s((mask.MergeP() * magic) & UINT64_C(0xfff0000000000000)) < 6)
			continue;

		std::fill(std::begin(attackUsed), std::IsEnd(attackUsed), CreateAllZeroBB());

		for (int i = 0; !fail && i < (1 << num1s); ++i) {
			const int shiftBits = g_rookShiftBits[square];
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
Bitboard RookAttackBb::RookBlockMaskCalc(const Square square) const {
	Bitboard result = g_fileMaskBb.GetSquareFileMask(square) ^ g_rankMaskBb.GetSquareRankMask(square);
	if (UtilSquare::ToFile(square) != FileA) { result &= ~g_fileMaskBb.GetFileMask<FileA>(); }
	if (UtilSquare::ToFile(square) != FileI) { result &= ~g_fileMaskBb.GetFileMask<FileI>(); }
	if (UtilSquare::ToRank(square) != Rank1) { result &= ~g_rankMaskBb.GetRankMask_rank(Rank1); }
	if (UtilSquare::ToRank(square) != Rank9) { result &= ~g_rankMaskBb.GetRankMask_rank(Rank9); }
	return result;
}


// Rook or Bishop の利きの範囲を調べて bitboard で返す。
// occupied  障害物があるマスが 1 の bitboard
Bitboard RookAttackBb::RookAttackCalc(const Square square, const Bitboard& occupied) const {
	const SquareDelta deltaArray[2][4] = { { DeltaN, DeltaS, DeltaE, DeltaW },{ DeltaNE, DeltaSE, DeltaSW, DeltaNW } };
	Bitboard result = Bitboard::CreateAllZeroBB();
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
	// 角か、飛車か
	auto* attacks = g_rookAttackBb.m_controllBb_;
	auto* attackIndex = g_rookAttackBb.m_rookAttackIndex;
	auto* blockMask = g_rookAttackBb.m_rookBlockMask_;
	auto* shift = this->m_rookShiftBits_;
#if defined HAVE_BMI2
#else
	auto* magic = this->m_rookMagic_;
#endif

	// 各マスについて
	int index = 0;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		blockMask[sq] = this->RookBlockMaskCalc(sq);
		attackIndex[sq] = index;

		const int num1s = this->m_rookBlockBits_[sq];
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
		g_rookAttackBb.m_controllBbToEdge_[sq] = g_rookAttackBb.GetControllBb(&Bitboard::CreateAllZeroBB(), sq);
	}
}
