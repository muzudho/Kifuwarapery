#pragma once


#include "../n110_square__/n110_500_utilSquare.hpp"

class Bitboard;


extern const int g_rookBlockBits[SquareNum];
extern const int g_bishopBlockBits[SquareNum];
extern const int g_rookShiftBits[SquareNum];
extern const int g_bishopShiftBits[SquareNum];
#if defined HAVE_BMI2
#else
extern const u64 g_rookMagic[SquareNum];
extern const u64 g_bishopMagic[SquareNum];
#endif

// �w�肵���ʒu�̑����� file �� bit �� shift ���A
// index �����߂�ׂɎg�p����B
const int g_slide[SquareNum] = {
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
extern Bitboard g_rookAttack[495616];
#else
extern Bitboard g_rookAttack[512000];
#endif
extern int g_rookAttackIndex[SquareNum];
// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
extern Bitboard g_bishopAttack[20224];
extern int g_bishopAttackIndex[SquareNum];
extern Bitboard g_rookBlockMask[SquareNum];
extern Bitboard g_bishopBlockMask[SquareNum];
// �������ߖ�������A���ʂȃ������������Ă���B
extern Bitboard g_lanceAttack[ColorNum][SquareNum][128];

extern Bitboard g_kingAttack[SquareNum];
extern Bitboard g_goldAttack[ColorNum][SquareNum];
extern Bitboard g_silverAttack[ColorNum][SquareNum];
extern Bitboard g_knightAttack[ColorNum][SquareNum];
extern Bitboard g_pawnAttack[ColorNum][SquareNum];

extern Bitboard g_betweenBB[SquareNum][SquareNum];

extern Bitboard g_rookAttackToEdge[SquareNum];
extern Bitboard g_bishopAttackToEdge[SquareNum];
extern Bitboard g_lanceAttackToEdge[ColorNum][SquareNum];

extern Bitboard g_goldCheckTable[ColorNum][SquareNum];
extern Bitboard g_silverCheckTable[ColorNum][SquareNum];
extern Bitboard g_knightCheckTable[ColorNum][SquareNum];
extern Bitboard g_lanceCheckTable[ColorNum][SquareNum];










extern const Bitboard g_setMaskBB[SquareNum];

class Bitboard {
private:
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	union {
		u64 m_p_[2];
		__m128i m_m_;
	};
#else
	u64 m_p_[2];	// m_p_[0] : ��肩�猩�āA1�ꂩ��7��܂ł��c�ɕ��ׂ�bit. 63bit�g�p. right �ƌĂԁB
				// m_p_[1] : ��肩�猩�āA8�ꂩ��1��܂ł��c�ɕ��ׂ�bit. 18bit�g�p. left  �ƌĂԁB
#endif

public:
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
	Bitboard& operator = (const Bitboard& rhs) {
		_mm_store_si128(&this->m_m_, rhs.m_m_);
		return *this;
	}
	Bitboard(const Bitboard& bb) {
		_mm_store_si128(&this->m_m_, bb.m_m_);
	}
#endif
	Bitboard() {}
	Bitboard(const u64 v0, const u64 v1) {
		this->m_p_[0] = v0;
		this->m_p_[1] = v1;
	}
	u64 GetP(const int index) const { return this->m_p_[index]; }
	void SetP(const int index, const u64 val) { this->m_p_[index] = val; }
	u64 MergeP() const { return this->GetP(0) | this->GetP(1); }
	bool isNot0() const {
#ifdef HAVE_SSE4
		return !(_mm_testz_si128(this->m_m_, _mm_set1_epi8(static_cast<char>(0xffu))));
#else
		return (this->MergeP() ? true : false);
#endif
	}
	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	bool andIsNot0(const Bitboard& bb) const {
#ifdef HAVE_SSE4
		return !(_mm_testz_si128(this->m_m_, bb.m_m_));
#else
		return (*this & bb).isNot0();
#endif
	}
	Bitboard operator ~ () const {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		Bitboard tmp;
		_mm_store_si128(&tmp.m_m_, _mm_andnot_si128(this->m_m_, _mm_set1_epi8(static_cast<char>(0xffu))));
		return tmp;
#else
		return Bitboard(~this->GetP(0), ~this->GetP(1));
#endif
	}
	Bitboard operator &= (const Bitboard& rhs) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_m_, _mm_and_si128(this->m_m_, rhs.m_m_));
#else
		this->m_p_[0] &= rhs.GetP(0);
		this->m_p_[1] &= rhs.GetP(1);
#endif
		return *this;
	}
	Bitboard operator |= (const Bitboard& rhs) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_m_, _mm_or_si128(this->m_m_, rhs.m_m_));
#else
		this->m_p_[0] |= rhs.GetP(0);
		this->m_p_[1] |= rhs.GetP(1);
#endif
		return *this;
	}
	Bitboard operator ^= (const Bitboard& rhs) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_m_, _mm_xor_si128(this->m_m_, rhs.m_m_));
#else
		this->m_p_[0] ^= rhs.GetP(0);
		this->m_p_[1] ^= rhs.GetP(1);
#endif
		return *this;
	}
	Bitboard operator <<= (const int i) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_m_, _mm_slli_epi64(this->m_m_, i));
#else
		this->m_p_[0] <<= i;
		this->m_p_[1] <<= i;
#endif
		return *this;
	}
	Bitboard operator >>= (const int i) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_m_, _mm_srli_epi64(this->m_m_, i));
#else
		this->m_p_[0] >>= i;
		this->m_p_[1] >>= i;
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
		return (_mm_testc_si128(_mm_cmpeq_epi8(this->m_m_, rhs.m_m_), _mm_set1_epi8(static_cast<char>(0xffu))) ? true : false);
#else
		return (this->GetP(0) == rhs.GetP(0)) && (this->GetP(1) == rhs.GetP(1));
#endif
	}
	bool operator != (const Bitboard& rhs) const { return !(*this == rhs); }
	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	Bitboard andEqualNot(const Bitboard& bb) {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		_mm_store_si128(&this->m_m_, _mm_andnot_si128(bb.m_m_, this->m_m_));
#else
		(*this) &= ~bb;
#endif
		return *this;
	}
	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	Bitboard notThisAnd(const Bitboard& bb) const {
#if defined (HAVE_SSE2) || defined (HAVE_SSE4)
		Bitboard temp;
		_mm_store_si128(&temp.m_m_, _mm_andnot_si128(this->m_m_, bb.m_m_));
		return temp;
#else
		return ~(*this) & bb;
#endif
	}
	bool isSet(const Square sq) const {
		assert(isInSquare(sq));
		return andIsNot0(g_setMaskBB[sq]);
	}
	void setBit(const Square sq) { *this |= g_setMaskBB[sq]; }
	void clearBit(const Square sq) { andEqualNot(g_setMaskBB[sq]); }
	void xorBit(const Square sq) { (*this) ^= g_setMaskBB[sq]; }
	void xorBit(const Square sq1, const Square sq2) { (*this) ^= (g_setMaskBB[sq1] | g_setMaskBB[sq2]); }
	// Bitboard �� right �������̗v�f�𒲂ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� right ���� 0 �łȂ����Ƃ�O��ɂ��Ă���B
	FORCE_INLINE Square firstOneRightFromI9() {
		const Square sq = static_cast<Square>(firstOneFromLSB(this->GetP(0)));
		// LSB ���̍ŏ��� 1 �� bit �� 0 �ɂ���
		this->m_p_[0] &= this->GetP(0) - 1;
		return sq;
	}
	// Bitboard �� left �������̗v�f�𒲂ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� left ���� 0 �łȂ����Ƃ�O��ɂ��Ă���B
	FORCE_INLINE Square firstOneLeftFromB9() {
		const Square sq = static_cast<Square>(firstOneFromLSB(this->GetP(1)) + 63);
		// LSB ���̍ŏ��� 1 �� bit �� 0 �ɂ���
		this->m_p_[1] &= this->GetP(1) - 1;
		return sq;
	}
	// Bitboard �� I9 ���� A1 �܂Œ��ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� allZeroBB() �łȂ����Ƃ�O��ɂ��Ă���B
	// VC++ �� _BitScanForward() �͓��͂� 0 �̂Ƃ��� 0 ��Ԃ��d�l�Ȃ̂ŁA
	// �ŏ��� 0 �łȂ������肷��̂͏������B
	FORCE_INLINE Square firstOneFromI9() {
		if (this->GetP(0)) {
			return firstOneRightFromI9();
		}
		return firstOneLeftFromB9();
	}
	// �Ԃ��ʒu�� 0 �ɂ��Ȃ��o�[�W�����B
	FORCE_INLINE Square constFirstOneRightFromI9() const { return static_cast<Square>(firstOneFromLSB(this->GetP(0))); }
	FORCE_INLINE Square constFirstOneLeftFromB9() const { return static_cast<Square>(firstOneFromLSB(this->GetP(1)) + 63); }
	FORCE_INLINE Square constFirstOneFromI9() const {
		if (this->GetP(0)) {
			return constFirstOneRightFromI9();
		}
		return constFirstOneLeftFromB9();
	}
	// Bitboard �� 1 �� bit �𐔂���B
	// Crossover �́AMergeP() ����� 1 �ł��� bit ���d�Ȃ�\��������Ȃ� true
	template <bool Crossover = true>
	int popCount() const { return (Crossover ? count1s(GetP(0)) + count1s(GetP(1)) : count1s(MergeP())); }
	// bit �� 1 ���������Ă��邩�ǂ����𔻒肷��B
	// Crossover �́AMergeP() ����� 1 �ł��� bit ���d�Ȃ�\��������Ȃ� true
	template <bool Crossover = true>
	bool isOneBit() const {
#if defined (HAVE_SSE42)
		return (this->popCount<Crossover>() == 1);
#else
		if (!this->isNot0()) {
			return false;
		}
		else if (this->GetP(0)) {
			return !((this->GetP(0) & (this->GetP(0) - 1)) | this->GetP(1));
		}
		return !(this->GetP(1) & (this->GetP(1) - 1));
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
				std::cout << (UINT64_C(1) & (this->GetP(part) >> makeSquare(f, r)));
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
	}

	// �w�肵���ʒu�� Bitboard �̂ǂ���� u64 �ϐ��̗v�f��
	static int part(const Square sq) { return static_cast<int>(C1 < sq); }

public://(^q^)
	inline static Bitboard setMaskBB(const Square sq) { return g_setMaskBB[sq]; }
	// ���ۂɎg�p���镔���̑S�� bit �������Ă��� Bitboard
	inline static Bitboard allOneBB() { return Bitboard(UINT64_C(0x7fffffffffffffff), UINT64_C(0x000000000003ffff)); }
	inline static Bitboard allZeroBB() { return Bitboard(0, 0); }

public://(^q^)

#if defined HAVE_BMI2
	// PEXT bitboard.
	inline u64 occupiedToIndex( const Bitboard& mask) const {
		return _pext_u64(this->MergeP(), mask.MergeP());
	}

	inline Bitboard rookAttack(const Square sq) const {
		const Bitboard block((*this) & g_rookBlockMask[sq]);
		return g_rookAttack[g_rookAttackIndex[sq] + occupiedToIndex(block, g_rookBlockMask[sq])];
	}

	inline Bitboard bishopAttack(const Square sq) const {
		const Bitboard block((*this) & g_bishopBlockMask[sq]);
		return g_bishopAttack[g_bishopAttackIndex[sq] + occupiedToIndex(block, g_bishopBlockMask[sq])];
	}

#else
	// magic bitboard.
	// magic number ���g���� block �̖͗l���痘���̃e�[�u���ւ̃C���f�b�N�X���Z�o
	inline u64 occupiedToIndex( const u64 magic, const int shiftBits) const {
		return (this->MergeP() * magic) >> shiftBits;
	}

	inline Bitboard rookAttack(const Square sq) const {
		const Bitboard block((*this) & g_rookBlockMask[sq]);
		return g_rookAttack[g_rookAttackIndex[sq] + block.occupiedToIndex(g_rookMagic[sq], g_rookShiftBits[sq])];
	}

	inline Bitboard bishopAttack(const Square sq) const {
		const Bitboard block((*this) & g_bishopBlockMask[sq]);
		return g_bishopAttack[g_bishopAttackIndex[sq] + block.occupiedToIndex(g_bishopMagic[sq], g_bishopShiftBits[sq])];
	}
#endif

	// todo: ���Ԃ̋؂��ǂ��ɂ��邩��ɕ������Ă���΁ABitboard �̕Е��̕ϐ������𒲂ׂ�Ηǂ��Ȃ�B
	inline Bitboard lanceAttack(const Color c, const Square sq) const {
		const int part = Bitboard::part(sq);
		const int index = ((*this).GetP(part) >> g_slide[sq]) & 127;
		return g_lanceAttack[c][sq][index];
	}

	// ��Ԃ̏c�����̗����B���Ԃ̗������g���Aindex �����ʉ����邱�Ƃō��������Ă���B
	inline Bitboard rookAttackFile(const Square sq) const {
		const int part = Bitboard::part(sq);
		const int index = ((*this).GetP(part) >> g_slide[sq]) & 127;
		return g_lanceAttack[Black][sq][index] | g_lanceAttack[White][sq][index];
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
		return g_kingAttack[sq];
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




inline Bitboard goldAttack(const Color c, const Square sq) { return g_goldAttack[c][sq]; }
inline Bitboard silverAttack(const Color c, const Square sq) { return g_silverAttack[c][sq]; }
inline Bitboard knightAttack(const Color c, const Square sq) { return g_knightAttack[c][sq]; }
inline Bitboard pawnAttack(const Color c, const Square sq) { return g_pawnAttack[c][sq]; }

// sq1, sq2 �̊�(sq1, sq2 �͊܂܂Ȃ�)�̃r�b�g�������� Bitboard
inline Bitboard betweenBB(const Square sq1, const Square sq2) { return g_betweenBB[sq1][sq2]; }
inline Bitboard rookAttackToEdge(const Square sq) { return g_rookAttackToEdge[sq]; }
inline Bitboard bishopAttackToEdge(const Square sq) { return g_bishopAttackToEdge[sq]; }
inline Bitboard lanceAttackToEdge(const Color c, const Square sq) { return g_lanceAttackToEdge[c][sq]; }
inline Bitboard dragonAttackToEdge(const Square sq) { return rookAttackToEdge(sq) | Bitboard::kingAttack(sq); }
inline Bitboard horseAttackToEdge(const Square sq) { return bishopAttackToEdge(sq) | Bitboard::kingAttack(sq); }
inline Bitboard goldCheckTable(const Color c, const Square sq) { return g_goldCheckTable[c][sq]; }
inline Bitboard silverCheckTable(const Color c, const Square sq) { return g_silverCheckTable[c][sq]; }
inline Bitboard knightCheckTable(const Color c, const Square sq) { return g_knightCheckTable[c][sq]; }
inline Bitboard lanceCheckTable(const Color c, const Square sq) { return g_lanceCheckTable[c][sq]; }
// todo: �e�[�u������������
inline Bitboard rookStepAttacks(const Square sq) { return goldAttack(Black, sq) & goldAttack(White, sq); }
// todo: �e�[�u������������
inline Bitboard bishopStepAttacks(const Square sq) { return silverAttack(Black, sq) & silverAttack(White, sq); }
// �O��3�����̈ʒu��Bitboard
inline Bitboard goldAndSilverAttacks(const Color c, const Square sq) { return goldAttack(c, sq) & silverAttack(c, sq); }
