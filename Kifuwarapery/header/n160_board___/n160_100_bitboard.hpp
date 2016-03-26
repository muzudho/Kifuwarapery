#pragma once


#include "../n120_brdEntry/n120_150_square.hpp"

class Bitboard;


extern const int g_arrRookBlockBits[SquareNum];
extern const int g_arrBishopBlockBits[SquareNum];
extern const int g_arrRookShiftBits[SquareNum];
extern const int g_arrBishopShiftBits[SquareNum];
#if defined HAVE_BMI2
#else
extern const u64 g_arrRookMagic[SquareNum];
extern const u64 g_arrBishopMagic[SquareNum];
#endif

// �w�肵���ʒu�̑����� file �� bit �� shift ���A
// index �����߂�ׂɎg�p����B
const int g_arrSlide[SquareNum] = {
	1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
	10, 10, 10, 10, 10, 10, 10, 10, 10,
	19, 19, 19, 19, 19, 19, 19, 19, 19,
	28, 28, 28, 28, 28, 28, 28, 28, 28,
	37, 37, 37, 37, 37, 37, 37, 37, 37,
	46, 46, 46, 46, 46, 46, 46, 46, 46,
	55, 55, 55, 55, 55, 55, 55, 55, 55,
	1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
	10, 10, 10, 10, 10, 10, 10, 10, 10
};

// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
#if defined HAVE_BMI2
extern Bitboard g_arrRookAttack[495616];
#else
extern Bitboard g_arrRookAttack[512000];
#endif
extern int g_arrRookAttackIndex[SquareNum];
// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
extern Bitboard g_arrBishopAttack[20224];
extern int g_arrBishopAttackIndex[SquareNum];
extern Bitboard g_arrRookBlockMask[SquareNum];
extern Bitboard g_arrBishopBlockMask[SquareNum];
// �������ߖ�������A���ʂȃ������������Ă���B
extern Bitboard g_arrLanceAttack[ColorNum][SquareNum][128];

extern Bitboard g_arrKingAttack[SquareNum];
extern Bitboard g_arrGoldAttack[ColorNum][SquareNum];
extern Bitboard g_arrSilverAttack[ColorNum][SquareNum];
extern Bitboard g_arrKnightAttack[ColorNum][SquareNum];
extern Bitboard g_arrPawnAttack[ColorNum][SquareNum];

extern Bitboard g_arrBetweenBB[SquareNum][SquareNum];

extern Bitboard g_arrRookAttackToEdge[SquareNum];
extern Bitboard g_arrBishopAttackToEdge[SquareNum];
extern Bitboard g_arrLanceAttackToEdge[ColorNum][SquareNum];

extern Bitboard g_arrGoldCheckTable[ColorNum][SquareNum];
extern Bitboard g_arrSilverCheckTable[ColorNum][SquareNum];
extern Bitboard g_arrKnightCheckTable[ColorNum][SquareNum];
extern Bitboard g_arrLanceCheckTable[ColorNum][SquareNum];










extern const Bitboard g_arrSetMaskBB[SquareNum];

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
		return andIsNot0(g_arrSetMaskBB[sq]);
	}
	void setBit(const Square sq) { *this |= g_arrSetMaskBB[sq]; }
	void clearBit(const Square sq) { andEqualNot(g_arrSetMaskBB[sq]); }
	void xorBit(const Square sq) { (*this) ^= g_arrSetMaskBB[sq]; }
	void xorBit(const Square sq1, const Square sq2) { (*this) ^= (g_arrSetMaskBB[sq1] | g_arrSetMaskBB[sq2]); }
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
	inline static Bitboard setMaskBB(const Square sq) { return g_arrSetMaskBB[sq]; }
	// ���ۂɎg�p���镔���̑S�� bit �������Ă��� Bitboard
	inline static Bitboard allOneBB() { return Bitboard(UINT64_C(0x7fffffffffffffff), UINT64_C(0x000000000003ffff)); }
	inline static Bitboard allZeroBB() { return Bitboard(0, 0); }

public://(^q^)

#if defined HAVE_BMI2
	// PEXT bitboard.
	inline u64 occupiedToIndex( const Bitboard& mask) const {
		return _pext_u64(this->merge(), mask.merge());
	}

	inline Bitboard rookAttack(const Square sq) const {
		const Bitboard block((*this) & g_arrRookBlockMask[sq]);
		return g_arrRookAttack[g_arrRookAttackIndex[sq] + occupiedToIndex(block, g_arrRookBlockMask[sq])];
	}

	inline Bitboard bishopAttack(const Square sq) const {
		const Bitboard block((*this) & g_arrBishopBlockMask[sq]);
		return g_arrBishopAttack[g_arrBishopAttackIndex[sq] + occupiedToIndex(block, g_arrBishopBlockMask[sq])];
	}

#else
	// magic bitboard.
	// magic number ���g���� block �̖͗l���痘���̃e�[�u���ւ̃C���f�b�N�X���Z�o
	inline u64 occupiedToIndex( const u64 magic, const int shiftBits) const {
		return (this->merge() * magic) >> shiftBits;
	}

	inline Bitboard rookAttack(const Square sq) const {
		const Bitboard block((*this) & g_arrRookBlockMask[sq]);
		return g_arrRookAttack[g_arrRookAttackIndex[sq] + block.occupiedToIndex(g_arrRookMagic[sq], g_arrRookShiftBits[sq])];
	}

	inline Bitboard bishopAttack(const Square sq) const {
		const Bitboard block((*this) & g_arrBishopBlockMask[sq]);
		return g_arrBishopAttack[g_arrBishopAttackIndex[sq] + block.occupiedToIndex(g_arrBishopMagic[sq], g_arrBishopShiftBits[sq])];
	}
#endif

	// todo: ���Ԃ̋؂��ǂ��ɂ��邩��ɕ������Ă���΁ABitboard �̕Е��̕ϐ������𒲂ׂ�Ηǂ��Ȃ�B
	inline Bitboard lanceAttack(const Color c, const Square sq) const {
		const int part = Bitboard::part(sq);
		const int index = ((*this).p(part) >> g_arrSlide[sq]) & 127;
		return g_arrLanceAttack[c][sq][index];
	}

	// ��Ԃ̏c�����̗����B���Ԃ̗������g���Aindex �����ʉ����邱�Ƃō��������Ă���B
	inline Bitboard rookAttackFile(const Square sq) const {
		const int part = Bitboard::part(sq);
		const int index = ((*this).p(part) >> g_arrSlide[sq]) & 127;
		return g_arrLanceAttack[Black][sq][index] | g_arrLanceAttack[White][sq][index];
	}

	// Bitboard �Œ��ڗ�����Ԃ��֐��B
	// 1�i�ڂɂ͕��͑��݂��Ȃ��̂ŁA1bit �V�t�g�ŕʂ̋؂ɍs�����Ƃ͂Ȃ��B
	// �������Afrom �ɕ��ȊO�̋�� Bitboard ������ƁA�ʂ̋؂̃r�b�g�������Ă��܂����Ƃ�����̂ŁA
	// �ʂ̋؂̃r�b�g�������Ȃ����A�����Ă����Ȃ������m�F���Ďg�p���邱�ƁB
	template <Color US> inline Bitboard pawnAttack() const { // this��from
		return (US == Black ? ((*this) >> 1) : ((*this) << 1));
	}

	// Bitboard �Œ��ڗ�����Ԃ��֐��B
	inline static Bitboard kingAttack(const Square sq) {
		return g_arrKingAttack[sq];
	}

	inline Bitboard dragonAttack(const Square sq) const
	{
		return this->rookAttack(sq) | Bitboard::kingAttack(sq);
	}

	inline Bitboard horseAttack(const Square sq) const
	{
		return this->bishopAttack(sq) | Bitboard::kingAttack(sq);
	}

	inline Bitboard queenAttack(const Square sq) const
	{
		return this->rookAttack(sq) | this->bishopAttack(sq);
	}

};




inline Bitboard goldAttack(const Color c, const Square sq) { return g_arrGoldAttack[c][sq]; }
inline Bitboard silverAttack(const Color c, const Square sq) { return g_arrSilverAttack[c][sq]; }
inline Bitboard knightAttack(const Color c, const Square sq) { return g_arrKnightAttack[c][sq]; }
inline Bitboard pawnAttack(const Color c, const Square sq) { return g_arrPawnAttack[c][sq]; }

// sq1, sq2 �̊�(sq1, sq2 �͊܂܂Ȃ�)�̃r�b�g�������� Bitboard
inline Bitboard betweenBB(const Square sq1, const Square sq2) { return g_arrBetweenBB[sq1][sq2]; }
inline Bitboard rookAttackToEdge(const Square sq) { return g_arrRookAttackToEdge[sq]; }
inline Bitboard bishopAttackToEdge(const Square sq) { return g_arrBishopAttackToEdge[sq]; }
inline Bitboard lanceAttackToEdge(const Color c, const Square sq) { return g_arrLanceAttackToEdge[c][sq]; }
inline Bitboard dragonAttackToEdge(const Square sq) { return rookAttackToEdge(sq) | Bitboard::kingAttack(sq); }
inline Bitboard horseAttackToEdge(const Square sq) { return bishopAttackToEdge(sq) | Bitboard::kingAttack(sq); }
inline Bitboard goldCheckTable(const Color c, const Square sq) { return g_arrGoldCheckTable[c][sq]; }
inline Bitboard silverCheckTable(const Color c, const Square sq) { return g_arrSilverCheckTable[c][sq]; }
inline Bitboard knightCheckTable(const Color c, const Square sq) { return g_arrKnightCheckTable[c][sq]; }
inline Bitboard lanceCheckTable(const Color c, const Square sq) { return g_arrLanceCheckTable[c][sq]; }
// todo: �e�[�u������������
inline Bitboard rookStepAttacks(const Square sq) { return goldAttack(Black, sq) & goldAttack(White, sq); }
// todo: �e�[�u������������
inline Bitboard bishopStepAttacks(const Square sq) { return silverAttack(Black, sq) & silverAttack(White, sq); }
// �O��3�����̈ʒu��Bitboard
inline Bitboard goldAndSilverAttacks(const Color c, const Square sq) { return goldAttack(c, sq) & silverAttack(c, sq); }
