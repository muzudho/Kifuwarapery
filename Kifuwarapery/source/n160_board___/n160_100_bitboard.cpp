// (^q^)外部に公開されているメソッドあり。
#include <iostream>	// std::cout
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n110_square__/n110_500_utilSquare.hpp"
#include "../../header/n160_board___/n160_400_bitboardAll.hpp"

using namespace std;

const Bitboard g_setMaskBB[SquareNum] = {
	Bitboard(UINT64_C(1) <<  0,                 0),  // 0 , I9
	Bitboard(UINT64_C(1) <<  1,                 0),  // 1 , I8
	Bitboard(UINT64_C(1) <<  2,                 0),  // 2 , I7
	Bitboard(UINT64_C(1) <<  3,                 0),  // 3 , I6
	Bitboard(UINT64_C(1) <<  4,                 0),  // 4 , I5
	Bitboard(UINT64_C(1) <<  5,                 0),  // 5 , I4
	Bitboard(UINT64_C(1) <<  6,                 0),  // 6 , I3
	Bitboard(UINT64_C(1) <<  7,                 0),  // 7 , I2
	Bitboard(UINT64_C(1) <<  8,                 0),  // 8 , I1
	Bitboard(UINT64_C(1) <<  9,                 0),  // 9 , H9
	Bitboard(UINT64_C(1) << 10,                 0),  // 10, H8
	Bitboard(UINT64_C(1) << 11,                 0),  // 11, H7
	Bitboard(UINT64_C(1) << 12,                 0),  // 12, H6
	Bitboard(UINT64_C(1) << 13,                 0),  // 13, H5
	Bitboard(UINT64_C(1) << 14,                 0),  // 14, H4
	Bitboard(UINT64_C(1) << 15,                 0),  // 15, H3
	Bitboard(UINT64_C(1) << 16,                 0),  // 16, H2
	Bitboard(UINT64_C(1) << 17,                 0),  // 17, H1
	Bitboard(UINT64_C(1) << 18,                 0),  // 18, G9
	Bitboard(UINT64_C(1) << 19,                 0),  // 19, G8
	Bitboard(UINT64_C(1) << 20,                 0),  // 20, G7
	Bitboard(UINT64_C(1) << 21,                 0),  // 21, G6
	Bitboard(UINT64_C(1) << 22,                 0),  // 22, G5
	Bitboard(UINT64_C(1) << 23,                 0),  // 23, G4
	Bitboard(UINT64_C(1) << 24,                 0),  // 24, G3
	Bitboard(UINT64_C(1) << 25,                 0),  // 25, G2
	Bitboard(UINT64_C(1) << 26,                 0),  // 26, G1
	Bitboard(UINT64_C(1) << 27,                 0),  // 27, F9
	Bitboard(UINT64_C(1) << 28,                 0),  // 28, F8
	Bitboard(UINT64_C(1) << 29,                 0),  // 29, F7
	Bitboard(UINT64_C(1) << 30,                 0),  // 30, F6
	Bitboard(UINT64_C(1) << 31,                 0),  // 31, F5
	Bitboard(UINT64_C(1) << 32,                 0),  // 32, F4
	Bitboard(UINT64_C(1) << 33,                 0),  // 33, F3
	Bitboard(UINT64_C(1) << 34,                 0),  // 34, F2
	Bitboard(UINT64_C(1) << 35,                 0),  // 35, F1
	Bitboard(UINT64_C(1) << 36,                 0),  // 36, E9
	Bitboard(UINT64_C(1) << 37,                 0),  // 37, E8
	Bitboard(UINT64_C(1) << 38,                 0),  // 38, E7
	Bitboard(UINT64_C(1) << 39,                 0),  // 39, E6
	Bitboard(UINT64_C(1) << 40,                 0),  // 40, E5
	Bitboard(UINT64_C(1) << 41,                 0),  // 41, E4
	Bitboard(UINT64_C(1) << 42,                 0),  // 42, E3
	Bitboard(UINT64_C(1) << 43,                 0),  // 43, E2
	Bitboard(UINT64_C(1) << 44,                 0),  // 44, E1
	Bitboard(UINT64_C(1) << 45,                 0),  // 45, D9
	Bitboard(UINT64_C(1) << 46,                 0),  // 46, D8
	Bitboard(UINT64_C(1) << 47,                 0),  // 47, D7
	Bitboard(UINT64_C(1) << 48,                 0),  // 48, D6
	Bitboard(UINT64_C(1) << 49,                 0),  // 49, D5
	Bitboard(UINT64_C(1) << 50,                 0),  // 50, D4
	Bitboard(UINT64_C(1) << 51,                 0),  // 51, D3
	Bitboard(UINT64_C(1) << 52,                 0),  // 52, D2
	Bitboard(UINT64_C(1) << 53,                 0),  // 53, D1
	Bitboard(UINT64_C(1) << 54,                 0),  // 54, C9
	Bitboard(UINT64_C(1) << 55,                 0),  // 55, C8
	Bitboard(UINT64_C(1) << 56,                 0),  // 56, C7
	Bitboard(UINT64_C(1) << 57,                 0),  // 57, C6
	Bitboard(UINT64_C(1) << 58,                 0),  // 58, C5
	Bitboard(UINT64_C(1) << 59,                 0),  // 59, C4
	Bitboard(UINT64_C(1) << 60,                 0),  // 60, C3
	Bitboard(UINT64_C(1) << 61,                 0),  // 61, C2
	Bitboard(UINT64_C(1) << 62,                 0),  // 62, C1
	Bitboard(                0, UINT64_C(1) <<  0),  // 63, B9
	Bitboard(                0, UINT64_C(1) <<  1),  // 64, B8
	Bitboard(                0, UINT64_C(1) <<  2),  // 65, B7
	Bitboard(                0, UINT64_C(1) <<  3),  // 66, B6
	Bitboard(                0, UINT64_C(1) <<  4),  // 67, B5
	Bitboard(                0, UINT64_C(1) <<  5),  // 68, B4
	Bitboard(                0, UINT64_C(1) <<  6),  // 69, B3
	Bitboard(                0, UINT64_C(1) <<  7),  // 70, B2
	Bitboard(                0, UINT64_C(1) <<  8),  // 71, B1
	Bitboard(                0, UINT64_C(1) <<  9),  // 72, A9
	Bitboard(                0, UINT64_C(1) << 10),  // 73, A8
	Bitboard(                0, UINT64_C(1) << 11),  // 74, A7
	Bitboard(                0, UINT64_C(1) << 12),  // 75, A6
	Bitboard(                0, UINT64_C(1) << 13),  // 76, A5
	Bitboard(                0, UINT64_C(1) << 14),  // 77, A4
	Bitboard(                0, UINT64_C(1) << 15),  // 78, A3
	Bitboard(                0, UINT64_C(1) << 16),  // 79, A2
	Bitboard(                0, UINT64_C(1) << 17)   // 80, A1
};


const Bitboard g_fileMask[FileNum] = {
	g_FileIMask, g_FileHMask, g_FileGMask, g_FileFMask, g_FileEMask, g_FileDMask, g_FileCMask, g_FileBMask, g_FileAMask
};

const Bitboard g_rankMask[RankNum] = {
	g_Rank9Mask, g_Rank8Mask, g_Rank7Mask, g_Rank6Mask, g_Rank5Mask, g_Rank4Mask, g_Rank3Mask, g_Rank2Mask, g_Rank1Mask
};

const Bitboard g_inFrontMask[ColorNum][RankNum] = {
	{ g_InFrontOfRank9Black, g_InFrontOfRank8Black, g_InFrontOfRank7Black, g_InFrontOfRank6Black, g_InFrontOfRank5Black, g_InFrontOfRank4Black, g_InFrontOfRank3Black, g_InFrontOfRank2Black, g_InFrontOfRank1Black },
	{ g_InFrontOfRank9White, g_InFrontOfRank8White, g_InFrontOfRank7White, g_InFrontOfRank6White, g_InFrontOfRank5White, g_InFrontOfRank4White, g_InFrontOfRank3White, g_InFrontOfRank2White, g_InFrontOfRank1White }
};




// これらは一度値を設定したら二度と変更しない。
// 本当は const 化したい。
Bitboard g_rookBlockMask[SquareNum];
Bitboard g_bishopAttack[20224];
int			g_bishopAttackIndex[SquareNum];
Bitboard g_bishopBlockMask[SquareNum];
Bitboard g_lanceAttack[ColorNum][SquareNum][128];

Bitboard g_kingAttack[SquareNum];
Bitboard g_goldAttack[ColorNum][SquareNum];
Bitboard g_silverAttack[ColorNum][SquareNum];
Bitboard g_knightAttack[ColorNum][SquareNum];
Bitboard g_pawnAttack[ColorNum][SquareNum];

Bitboard g_betweenBB[SquareNum][SquareNum];

Bitboard g_rookAttackToEdge[SquareNum];
Bitboard g_bishopAttackToEdge[SquareNum];
Bitboard g_lanceAttackToEdge[ColorNum][SquareNum];

Bitboard g_goldCheckTable[ColorNum][SquareNum];
Bitboard g_silverCheckTable[ColorNum][SquareNum];
Bitboard g_knightCheckTable[ColorNum][SquareNum];
Bitboard g_lanceCheckTable[ColorNum][SquareNum];

Bitboard::Bitboard(const u64 v0, const u64 v1)
{
	this->m_p_[0] = v0;
	this->m_p_[1] = v1;
}

u64 Bitboard::GetP(const int index) const
{
	return this->m_p_[index];
}

void Bitboard::SetP(const int index, const u64 val)
{
	this->m_p_[index] = val;
}

u64 Bitboard::MergeP() const
{
	return this->GetP(0) | this->GetP(1);
}

bool Bitboard::IsNot0() const
{
#ifdef HAVE_SSE4
	return !(_mm_testz_si128(this->m_m_, _mm_set1_epi8(static_cast<char>(0xffu))));
#else
	return (this->MergeP() ? true : false);
#endif
}

bool Bitboard::AndIsNot0(const Bitboard & bb) const
{
#ifdef HAVE_SSE4
	return !(_mm_testz_si128(this->m_m_, bb.m_m_));
#else
	return (*this & bb).IsNot0();
#endif
}

Bitboard Bitboard::operator~() const
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	Bitboard tmp;
	_mm_store_si128(&tmp.m_m_, _mm_andnot_si128(this->m_m_, _mm_set1_epi8(static_cast<char>(0xffu))));
	return tmp;
#else
	return Bitboard(~this->GetP(0), ~this->GetP(1));
#endif
}

Bitboard Bitboard::operator&=(const Bitboard & rhs)
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	_mm_store_si128(&this->m_m_, _mm_and_si128(this->m_m_, rhs.m_m_));
#else
	this->m_p_[0] &= rhs.GetP(0);
	this->m_p_[1] &= rhs.GetP(1);
#endif
	return *this;
}

Bitboard Bitboard::operator|=(const Bitboard & rhs)
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	_mm_store_si128(&this->m_m_, _mm_or_si128(this->m_m_, rhs.m_m_));
#else
	this->m_p_[0] |= rhs.GetP(0);
	this->m_p_[1] |= rhs.GetP(1);
#endif
	return *this;
}

Bitboard Bitboard::operator^=(const Bitboard & rhs)
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	_mm_store_si128(&this->m_m_, _mm_xor_si128(this->m_m_, rhs.m_m_));
#else
	this->m_p_[0] ^= rhs.GetP(0);
	this->m_p_[1] ^= rhs.GetP(1);
#endif
	return *this;
}

Bitboard Bitboard::operator<<=(const int i)
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	_mm_store_si128(&this->m_m_, _mm_slli_epi64(this->m_m_, i));
#else
	this->m_p_[0] <<= i;
	this->m_p_[1] <<= i;
#endif
	return *this;
}

Bitboard Bitboard::operator>>=(const int i)
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	_mm_store_si128(&this->m_m_, _mm_srli_epi64(this->m_m_, i));
#else
	this->m_p_[0] >>= i;
	this->m_p_[1] >>= i;
#endif
	return *this;
}

Bitboard Bitboard::operator&(const Bitboard & rhs) const
{
	return Bitboard(*this) &= rhs;
}

Bitboard Bitboard::operator|(const Bitboard & rhs) const
{
	return Bitboard(*this) |= rhs;
}

Bitboard Bitboard::operator^(const Bitboard & rhs) const
{
	return Bitboard(*this) ^= rhs;
}

Bitboard Bitboard::operator<<(const int i) const
{
	return Bitboard(*this) <<= i;
}

Bitboard Bitboard::operator>>(const int i) const
{
	return Bitboard(*this) >>= i;
}

bool Bitboard::operator==(const Bitboard & rhs) const
{
#ifdef HAVE_SSE4
	return (_mm_testc_si128(_mm_cmpeq_epi8(this->m_m_, rhs.m_m_), _mm_set1_epi8(static_cast<char>(0xffu))) ? true : false);
#else
	return (this->GetP(0) == rhs.GetP(0)) && (this->GetP(1) == rhs.GetP(1));
#endif
}

bool Bitboard::operator!=(const Bitboard & rhs) const
{
	return !(*this == rhs);
}

Bitboard Bitboard::AndEqualNot(const Bitboard & bb)
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	_mm_store_si128(&this->m_m_, _mm_andnot_si128(bb.m_m_, this->m_m_));
#else
	(*this) &= ~bb;
#endif
	return *this;
}

Bitboard Bitboard::NotThisAnd(const Bitboard & bb) const
{
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	Bitboard temp;
	_mm_store_si128(&temp.m_m_, _mm_andnot_si128(this->m_m_, bb.m_m_));
	return temp;
#else
	return ~(*this) & bb;
#endif
}

bool Bitboard::IsSet(const Square sq) const
{
	assert(UtilSquare::ContainsOf(sq));
	return AndIsNot0(g_setMaskBB[sq]);
}

void Bitboard::SetBit(const Square sq)
{
	*this |= g_setMaskBB[sq];
}

void Bitboard::ClearBit(const Square sq)
{
	AndEqualNot(g_setMaskBB[sq]);
}

void Bitboard::XorBit(const Square sq)
{
	(*this) ^= g_setMaskBB[sq];
}

void Bitboard::XorBit(const Square sq1, const Square sq2)
{
	(*this) ^= (g_setMaskBB[sq1] | g_setMaskBB[sq2]);
}

void Bitboard::PrintBoard() const
{
	std::cout << "   A  B  C  D  E  F  G  H  I\n";
	for (Rank r = Rank9; r < RankNum; ++r) {
		std::cout << (9 - r);
		for (File f = FileA; FileI <= f; --f) {
			std::cout << (this->IsSet(UtilSquare::FromFileRank(f, r)) ? "  X" : "  .");
		}
		std::cout << "\n";
	}

	std::cout << std::endl;
}

void Bitboard::PrintTable(const int part) const
{
	for (Rank r = Rank9; r < RankNum; ++r) {
		for (File f = FileC; FileI <= f; --f) {
			std::cout << (UINT64_C(1) & (this->GetP(part) >> UtilSquare::FromFileRank(f, r)));
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int Bitboard::Part(const Square sq)
{
	return static_cast<int>(C1 < sq);
}
