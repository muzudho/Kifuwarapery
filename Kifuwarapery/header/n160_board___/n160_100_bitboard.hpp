#pragma once


#include "../n120_brdEntry/n120_150_square.hpp"


class Bitboard;
extern const Bitboard SetMaskBB[SquareNum];

class Bitboard {
public:
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	Bitboard& operator = (const Bitboard& rhs) {
		_mm_store_si128(&this->m_, rhs.m_);
		return *this;
	}
	Bitboard(const Bitboard& bb) {
		_mm_store_si128(&this->m_, bb.m_);
	}
#endif
	Bitboard() {}
	Bitboard(const u64 v0, const u64 v1) {
		this->p_[0] = v0;
		this->p_[1] = v1;
	}
	u64 p(const int index) const { return p_[index]; }
	void set(const int index, const u64 val) { p_[index] = val; }
	u64 merge() const { return this->p(0) | this->p(1); }
	bool isNot0() const {
#ifdef HAVE_SSE4
		return !(_mm_testz_si128(this->m_, _mm_set1_epi8(static_cast<char>(0xffu))));
#else
		return (this->merge() ? true : false);
#endif
	}
	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	bool andIsNot0(const Bitboard& bb) const {
#ifdef HAVE_SSE4
		return !(_mm_testz_si128(this->m_, bb.m_));
#else
		return (*this & bb).isNot0();
#endif
	}
	Bitboard operator ~ () const {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		Bitboard tmp;
		_mm_store_si128(&tmp.m_, _mm_andnot_si128(this->m_, _mm_set1_epi8(static_cast<char>(0xffu))));
		return tmp;
#else
		return Bitboard(~this->p(0), ~this->p(1));
#endif
	}
	Bitboard operator &= (const Bitboard& rhs) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_, _mm_and_si128(this->m_, rhs.m_));
#else
		this->p_[0] &= rhs.p(0);
		this->p_[1] &= rhs.p(1);
#endif
		return *this;
	}
	Bitboard operator |= (const Bitboard& rhs) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_, _mm_or_si128(this->m_, rhs.m_));
#else
		this->p_[0] |= rhs.p(0);
		this->p_[1] |= rhs.p(1);
#endif
		return *this;
	}
	Bitboard operator ^= (const Bitboard& rhs) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_, _mm_xor_si128(this->m_, rhs.m_));
#else
		this->p_[0] ^= rhs.p(0);
		this->p_[1] ^= rhs.p(1);
#endif
		return *this;
	}
	Bitboard operator <<= (const int i) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_, _mm_slli_epi64(this->m_, i));
#else
		this->p_[0] <<= i;
		this->p_[1] <<= i;
#endif
		return *this;
	}
	Bitboard operator >>= (const int i) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_, _mm_srli_epi64(this->m_, i));
#else
		this->p_[0] >>= i;
		this->p_[1] >>= i;
#endif
		return *this;
	}
	Bitboard operator & (const Bitboard& rhs) const { return Bitboard(*this) &= rhs; }
	Bitboard operator | (const Bitboard& rhs) const { return Bitboard(*this) |= rhs; }
	Bitboard operator ^ (const Bitboard& rhs) const { return Bitboard(*this) ^= rhs; }
	Bitboard operator << (const int i) const { return Bitboard(*this) <<= i; }
	Bitboard operator >> (const int i) const { return Bitboard(*this) >>= i; }
	bool operator == (const Bitboard& rhs) const {
#ifdef HAVE_SSE4
		return (_mm_testc_si128(_mm_cmpeq_epi8(this->m_, rhs.m_), _mm_set1_epi8(static_cast<char>(0xffu))) ? true : false);
#else
		return (this->p(0) == rhs.p(0)) && (this->p(1) == rhs.p(1));
#endif
	}
	bool operator != (const Bitboard& rhs) const { return !(*this == rhs); }
	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	Bitboard andEqualNot(const Bitboard& bb) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_, _mm_andnot_si128(bb.m_, this->m_));
#else
		(*this) &= ~bb;
#endif
		return *this;
	}
	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	Bitboard notThisAnd(const Bitboard& bb) const {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		Bitboard temp;
		_mm_store_si128(&temp.m_, _mm_andnot_si128(this->m_, bb.m_));
		return temp;
#else
		return ~(*this) & bb;
#endif
	}
	bool isSet(const Square sq) const {
		assert(isInSquare(sq));
		return andIsNot0(SetMaskBB[sq]);
	}
	void setBit(const Square sq) { *this |= SetMaskBB[sq]; }
	void clearBit(const Square sq) { andEqualNot(SetMaskBB[sq]); }
	void xorBit(const Square sq) { (*this) ^= SetMaskBB[sq]; }
	void xorBit(const Square sq1, const Square sq2) { (*this) ^= (SetMaskBB[sq1] | SetMaskBB[sq2]); }
	// Bitboard �� right �������̗v�f�𒲂ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� right ���� 0 �łȂ����Ƃ�O��ɂ��Ă���B
	FORCE_INLINE Square firstOneRightFromI9() {
		const Square sq = static_cast<Square>(firstOneFromLSB(this->p(0)));
		// LSB ���̍ŏ��� 1 �� bit �� 0 �ɂ���
		this->p_[0] &= this->p(0) - 1;
		return sq;
	}
	// Bitboard �� left �������̗v�f�𒲂ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� left ���� 0 �łȂ����Ƃ�O��ɂ��Ă���B
	FORCE_INLINE Square firstOneLeftFromB9() {
		const Square sq = static_cast<Square>(firstOneFromLSB(this->p(1)) + 63);
		// LSB ���̍ŏ��� 1 �� bit �� 0 �ɂ���
		this->p_[1] &= this->p(1) - 1;
		return sq;
	}
	// Bitboard �� I9 ���� A1 �܂Œ��ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� allZeroBB() �łȂ����Ƃ�O��ɂ��Ă���B
	// VC++ �� _BitScanForward() �͓��͂� 0 �̂Ƃ��� 0 ��Ԃ��d�l�Ȃ̂ŁA
	// �ŏ��� 0 �łȂ������肷��̂͏������B
	FORCE_INLINE Square firstOneFromI9() {
		if (this->p(0)) {
			return firstOneRightFromI9();
		}
		return firstOneLeftFromB9();
	}
	// �Ԃ��ʒu�� 0 �ɂ��Ȃ��o�[�W�����B
	FORCE_INLINE Square constFirstOneRightFromI9() const { return static_cast<Square>(firstOneFromLSB(this->p(0))); }
	FORCE_INLINE Square constFirstOneLeftFromB9() const { return static_cast<Square>(firstOneFromLSB(this->p(1)) + 63); }
	FORCE_INLINE Square constFirstOneFromI9() const {
		if (this->p(0)) {
			return constFirstOneRightFromI9();
		}
		return constFirstOneLeftFromB9();
	}
	// Bitboard �� 1 �� bit �𐔂���B
	// Crossover �́Amerge() ����� 1 �ł��� bit ���d�Ȃ�\��������Ȃ� true
	template <bool Crossover = true>
	int popCount() const { return (Crossover ? count1s(p(0)) + count1s(p(1)) : count1s(merge())); }
	// bit �� 1 ���������Ă��邩�ǂ����𔻒肷��B
	// Crossover �́Amerge() ����� 1 �ł��� bit ���d�Ȃ�\��������Ȃ� true
	template <bool Crossover = true>
	bool isOneBit() const {
#if defined (HAVE_SSE42)
		return (this->popCount<Crossover>() == 1);
#else
		if (!this->isNot0()) {
			return false;
		}
		else if (this->p(0)) {
			return !((this->p(0) & (this->p(0) - 1)) | this->p(1));
		}
		return !(this->p(1) & (this->p(1) - 1));
#endif
	}

	// for debug
	void printBoard() const {
		std::cout << "   A  B  C  D  E  F  G  H  I\n";
		for (Rank r = Rank9; r < RankNum; ++r) {
			std::cout << (9 - r);
			for (File f = FileA; FileI <= f; --f) {
				std::cout << (this->isSet(makeSquare(f, r)) ? "  X" : "  .");
			}
			std::cout << "\n";
		}

		std::cout << std::endl;
	}

	void printTable(const int part) const {
		for (Rank r = Rank9; r < RankNum; ++r) {
			for (File f = FileC; FileI <= f; --f) {
				std::cout << (UINT64_C(1) & (this->p(part) >> makeSquare(f, r)));
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	// �w�肵���ʒu�� Bitboard �̂ǂ���� u64 �ϐ��̗v�f��
	static int part(const Square sq) { return static_cast<int>(C1 < sq); }

private:
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	union {
		u64 p_[2];
		__m128i m_;
	};
#else
	u64 p_[2];	// p_[0] : ��肩�猩�āA1�ꂩ��7��܂ł��c�ɕ��ׂ�bit. 63bit�g�p. right �ƌĂԁB
				// p_[1] : ��肩�猩�āA8�ꂩ��1��܂ł��c�ɕ��ׂ�bit. 18bit�g�p. left  �ƌĂԁB
#endif

public://(^q^)
	inline static Bitboard setMaskBB(const Square sq) { return SetMaskBB[sq]; }
	// ���ۂɎg�p���镔���̑S�� bit �������Ă��� Bitboard
	inline static Bitboard allOneBB() { return Bitboard(UINT64_C(0x7fffffffffffffff), UINT64_C(0x000000000003ffff)); }
	inline static Bitboard allZeroBB() { return Bitboard(0, 0); }
};


