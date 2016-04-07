// (^q^)外部に公開されているメソッドあり。
#include <iostream>	// std::cout
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n110_square__/n110_500_utilSquare.hpp"
#include "../../header/n160_board___/n160_600_bitboardAll.hpp"

using namespace std;




const Bitboard g_inFrontMask[ColorNum][RankNum] = {
	{ g_InFrontOfRank9Black, g_InFrontOfRank8Black, g_InFrontOfRank7Black, g_InFrontOfRank6Black, g_InFrontOfRank5Black, g_InFrontOfRank4Black, g_InFrontOfRank3Black, g_InFrontOfRank2Black, g_InFrontOfRank1Black },
	{ g_InFrontOfRank9White, g_InFrontOfRank8White, g_InFrontOfRank7White, g_InFrontOfRank6White, g_InFrontOfRank5White, g_InFrontOfRank4White, g_InFrontOfRank3White, g_InFrontOfRank2White, g_InFrontOfRank1White }
};







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

bool Bitboard::Exists1Bit() const
{
#ifdef HAVE_SSE4
	return !(_mm_testz_si128(this->m_m_, _mm_set1_epi8(static_cast<char>(0xffu))));
#else
	return (this->MergeP() ? true : false);
#endif
}

bool Bitboard::AndIsNot0( const Bitboard & bb) const
{
#ifdef HAVE_SSE4
	return !(_mm_testz_si128(this->m_m_, bb.m_m_));
#else
	return (*this & bb).Exists1Bit();
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
