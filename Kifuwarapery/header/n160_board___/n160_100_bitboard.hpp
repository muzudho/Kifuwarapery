#pragma once


#include "../n110_square__/n110_500_utilSquare.hpp"
#include "../n160_board___/n160_050_configBits.hpp"

class Bitboard;



// �������ߖ�ׁ̈A1�����z��ɂ��Ė��ʂ������悤�ɂ��Ă���B
extern Bitboard g_bishopAttack		[20224];
extern int		g_bishopAttackIndex	[SquareNum];
extern Bitboard g_rookBlockMask		[SquareNum];
extern Bitboard g_bishopBlockMask	[SquareNum];
// �������ߖ�������A���ʂȃ������������Ă���B
extern Bitboard g_lanceAttack		[ColorNum]	[SquareNum][128];

extern Bitboard g_kingAttack		[SquareNum];
extern Bitboard g_goldAttack		[ColorNum]	[SquareNum];
extern Bitboard g_silverAttack		[ColorNum]	[SquareNum];
extern Bitboard g_knightAttack		[ColorNum]	[SquareNum];
extern Bitboard g_pawnAttack		[ColorNum]	[SquareNum];

extern Bitboard g_betweenBB			[SquareNum]	[SquareNum];

extern Bitboard g_rookAttackToEdge	[SquareNum];
extern Bitboard g_bishopAttackToEdge[SquareNum];
extern Bitboard g_lanceAttackToEdge	[ColorNum]	[SquareNum];

extern Bitboard g_goldCheckTable	[ColorNum]	[SquareNum];
extern Bitboard g_silverCheckTable	[ColorNum]	[SquareNum];
extern Bitboard g_knightCheckTable	[ColorNum]	[SquareNum];
extern Bitboard g_lanceCheckTable	[ColorNum]	[SquareNum];



extern const Bitboard g_setMaskBB	[SquareNum];

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

	Bitboard(const u64 v0, const u64 v1);

	u64 GetP(const int index) const;

	void SetP(const int index, const u64 val);

	u64 MergeP() const;

	bool IsNot0() const;

	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	bool AndIsNot0(const Bitboard& bb) const;

	Bitboard operator ~ () const;

	Bitboard operator &= (const Bitboard& rhs);

	Bitboard operator |= (const Bitboard& rhs);

	Bitboard operator ^= (const Bitboard& rhs);

	Bitboard operator <<= (const int i);

	Bitboard operator >>= (const int i);

	Bitboard operator & (const Bitboard& rhs) const;

	Bitboard operator | (const Bitboard& rhs) const;

	Bitboard operator ^ (const Bitboard& rhs) const;

	Bitboard operator << (const int i) const;

	Bitboard operator >> (const int i) const;

	bool operator == (const Bitboard& rhs) const;

	bool operator != (const Bitboard& rhs) const;

	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	Bitboard AndEqualNot(const Bitboard& bb);

	// ����̓R�[�h������Ȃ邯�ǎd���Ȃ��B
	Bitboard NotThisAnd(const Bitboard& bb) const;

	bool IsSet(const Square sq) const;

	void SetBit(const Square sq);

	void ClearBit(const Square sq);

	void XorBit(const Square sq);

	void XorBit(const Square sq1, const Square sq2);

	// Bitboard �� right �������̗v�f�𒲂ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� right ���� 0 �łȂ����Ƃ�O��ɂ��Ă���B
	FORCE_INLINE Square FirstOneRightFromI9() {
		const Square sq = static_cast<Square>(firstOneFromLSB(this->GetP(0)));
		// LSB ���̍ŏ��� 1 �� bit �� 0 �ɂ���
		this->m_p_[0] &= this->GetP(0) - 1;
		return sq;
	}


	// Bitboard �� left �������̗v�f�𒲂ׂāA�ŏ��� 1 �ł���}�X�� index ��Ԃ��B
	// ���̃}�X�� 0 �ɂ���B
	// Bitboard �� left ���� 0 �łȂ����Ƃ�O��ɂ��Ă���B
	FORCE_INLINE Square FirstOneLeftFromB9() {
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
	FORCE_INLINE Square FirstOneFromI9()
	{
		if (this->GetP(0)) {
			return FirstOneRightFromI9();
		}
		return FirstOneLeftFromB9();
	}

	// �Ԃ��ʒu�� 0 �ɂ��Ȃ��o�[�W�����B
	FORCE_INLINE Square ConstFirstOneRightFromI9() const
	{
		return static_cast<Square>(firstOneFromLSB(this->GetP(0)));
	}

	FORCE_INLINE Square ConstFirstOneLeftFromB9() const
	{
		return static_cast<Square>(firstOneFromLSB(this->GetP(1)) + 63);
	}

	FORCE_INLINE Square ConstFirstOneFromI9() const
	{
		if (this->GetP(0)) {
			return ConstFirstOneRightFromI9();
		}
		return ConstFirstOneLeftFromB9();
	}

	// Bitboard �� 1 �� bit �𐔂���B
	// Crossover �́AMergeP() ����� 1 �ł��� bit ���d�Ȃ�\��������Ȃ� true
	template <bool Crossover = true>
	int PopCount() const { return (Crossover ? count1s(GetP(0)) + count1s(GetP(1)) : count1s(MergeP())); }

	// bit �� 1 ���������Ă��邩�ǂ����𔻒肷��B
	// Crossover �́AMergeP() ����� 1 �ł��� bit ���d�Ȃ�\��������Ȃ� true
	template <bool Crossover = true>
	bool IsOneBit() const {
#if defined (HAVE_SSE42)
		return (this->PopCount<Crossover>() == 1);
#else
		if (!this->IsNot0()) {
			return false;
		}
		else if (this->GetP(0)) {
			return !((this->GetP(0) & (this->GetP(0) - 1)) | this->GetP(1));
		}
		return !(this->GetP(1) & (this->GetP(1) - 1));
#endif
	}

	// for debug
	void PrintBoard() const;

	void PrintTable(const int part) const;

	// �w�肵���ʒu�� Bitboard �̂ǂ���� u64 �ϐ��̗v�f��
	static int Part(const Square sq);

public://(^q^)
	inline static Bitboard SetMaskBB(const Square sq) { return g_setMaskBB[sq]; }

	// ���ۂɎg�p���镔���̑S�� bit �������Ă��� Bitboard
	inline static Bitboard AllOneBB() { return Bitboard(UINT64_C(0x7fffffffffffffff), UINT64_C(0x000000000003ffff)); }

	inline static Bitboard AllZeroBB() { return Bitboard(0, 0); }

public://(^q^)

#if defined HAVE_BMI2
	// PEXT bitboard.
	inline u64 OccupiedToIndex( const Bitboard& mask) const {
		return _pext_u64(this->MergeP(), mask.MergeP());
	}

#else
	// magic bitboard.
	// magic number ���g���� block �̖͗l���痘���̃e�[�u���ւ̃C���f�b�N�X���Z�o
	inline u64 OccupiedToIndex( const u64 magic, const int shiftBits) const {
		return (this->MergeP() * magic) >> shiftBits;
	}
#endif

	// todo: ���Ԃ̋؂��ǂ��ɂ��邩��ɕ������Ă���΁ABitboard �̕Е��̕ϐ������𒲂ׂ�Ηǂ��Ȃ�B
	inline Bitboard LanceAttack(const Color c, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*this).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return g_lanceAttack[c][sq][index];
	}

	// ��Ԃ̏c�����̗����B���Ԃ̗������g���Aindex �����ʉ����邱�Ƃō��������Ă���B
	inline Bitboard RookAttackFile(const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*this).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return g_lanceAttack[Black][sq][index] | g_lanceAttack[White][sq][index];
	}

	// Bitboard �Œ��ڗ�����Ԃ��֐��B
	// 1�i�ڂɂ͕��͑��݂��Ȃ��̂ŁA1bit �V�t�g�ŕʂ̋؂ɍs�����Ƃ͂Ȃ��B
	// �������Afrom �ɕ��ȊO�̋�� Bitboard ������ƁA�ʂ̋؂̃r�b�g�������Ă��܂����Ƃ�����̂ŁA
	// �ʂ̋؂̃r�b�g�������Ȃ����A�����Ă����Ȃ������m�F���Ďg�p���邱�ƁB
	template <Color US> inline Bitboard PawnAttack() const { // this��from
		return (US == Black ? ((*this) >> 1) : ((*this) << 1));
	}

	// Bitboard �Œ��ڗ�����Ԃ��֐��B
	inline static Bitboard KingAttack(const Square sq) {
		return g_kingAttack[sq];
	}

	static inline Bitboard GoldAttack(const Color c, const Square sq) { return g_goldAttack[c][sq]; }
	static inline Bitboard SilverAttack(const Color c, const Square sq) { return g_silverAttack[c][sq]; }
	static inline Bitboard KnightAttack(const Color c, const Square sq) { return g_knightAttack[c][sq]; }
	static inline Bitboard PawnAttack(const Color c, const Square sq) { return g_pawnAttack[c][sq]; }


	// sq1, sq2 �̊�(sq1, sq2 �͊܂܂Ȃ�)�̃r�b�g�������� Bitboard
	static inline Bitboard BetweenBB			(const Square sq1	, const Square sq2	) { return g_betweenBB			[sq1][sq2]; }
	static inline Bitboard RookAttackToEdge		(const Square sq						) { return g_rookAttackToEdge	[sq];		}
	static inline Bitboard BishopAttackToEdge	(const Square sq						) { return g_bishopAttackToEdge	[sq];		}
	static inline Bitboard LanceAttackToEdge	(const Color c		, const Square sq	) { return g_lanceAttackToEdge	[c][sq];	}
	static inline Bitboard DragonAttackToEdge	(const Square sq						) { return Bitboard::RookAttackToEdge(sq) | Bitboard::KingAttack(sq); }
	static inline Bitboard HorseAttackToEdge	(const Square sq						) { return Bitboard::BishopAttackToEdge(sq) | Bitboard::KingAttack(sq); }
	static inline Bitboard GoldCheckTable		(const Color c		, const Square sq	) { return g_goldCheckTable		[c][sq];	}
	static inline Bitboard SilverCheckTable		(const Color c		, const Square sq	) { return g_silverCheckTable	[c][sq];	}
	static inline Bitboard KnightCheckTable		(const Color c		, const Square sq	) { return g_knightCheckTable	[c][sq];	}
	static inline Bitboard LanceCheckTable		(const Color c		, const Square sq	) { return g_lanceCheckTable	[c][sq];	}
	// todo: �e�[�u������������
	static inline Bitboard RookStepAttacks		(const Square sq						) { return Bitboard::GoldAttack(Black, sq) & Bitboard::GoldAttack(White, sq); }
	// todo: �e�[�u������������
	static inline Bitboard BishopStepAttacks	(const Square sq						) { return Bitboard::SilverAttack(Black, sq) & Bitboard::SilverAttack(White, sq); }
	// �O��3�����̈ʒu��Bitboard
	static inline Bitboard GoldAndSilverAttacks	(const Color c		, const Square sq	) { return Bitboard::GoldAttack(c, sq) & Bitboard::SilverAttack(c, sq); }

};



