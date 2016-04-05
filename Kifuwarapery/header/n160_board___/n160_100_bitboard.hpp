#pragma once


#include "../n110_square__/n110_500_utilSquare.hpp"
#include "../n160_board___/n160_050_configBits.hpp"

class Bitboard;



// メモリ節約の為、1次元配列にして無駄が無いようにしている。
extern Bitboard g_bishopAttack		[20224];
extern int		g_bishopAttackIndex	[SquareNum];
extern Bitboard g_rookBlockMask		[SquareNum];
extern Bitboard g_bishopBlockMask	[SquareNum];
// メモリ節約をせず、無駄なメモリを持っている。
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
	u64 m_p_[2];	// m_p_[0] : 先手から見て、1一から7九までを縦に並べたbit. 63bit使用. right と呼ぶ。
					// m_p_[1] : 先手から見て、8一から1九までを縦に並べたbit. 18bit使用. left  と呼ぶ。
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

	// これはコードが見難くなるけど仕方ない。
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

	// これはコードが見難くなるけど仕方ない。
	Bitboard AndEqualNot(const Bitboard& bb);

	// これはコードが見難くなるけど仕方ない。
	Bitboard NotThisAnd(const Bitboard& bb) const;

	bool IsSet(const Square sq) const;

	void SetBit(const Square sq);

	void ClearBit(const Square sq);

	void XorBit(const Square sq);

	void XorBit(const Square sq1, const Square sq2);

	// Bitboard の right 側だけの要素を調べて、最初に 1 であるマスの index を返す。
	// そのマスを 0 にする。
	// Bitboard の right 側が 0 でないことを前提にしている。
	FORCE_INLINE Square FirstOneRightFromI9() {
		const Square sq = static_cast<Square>(firstOneFromLSB(this->GetP(0)));
		// LSB 側の最初の 1 の bit を 0 にする
		this->m_p_[0] &= this->GetP(0) - 1;
		return sq;
	}


	// Bitboard の left 側だけの要素を調べて、最初に 1 であるマスの index を返す。
	// そのマスを 0 にする。
	// Bitboard の left 側が 0 でないことを前提にしている。
	FORCE_INLINE Square FirstOneLeftFromB9() {
		const Square sq = static_cast<Square>(firstOneFromLSB(this->GetP(1)) + 63);
		// LSB 側の最初の 1 の bit を 0 にする
		this->m_p_[1] &= this->GetP(1) - 1;
		return sq;
	}


	// Bitboard を I9 から A1 まで調べて、最初に 1 であるマスの index を返す。
	// そのマスを 0 にする。
	// Bitboard が allZeroBB() でないことを前提にしている。
	// VC++ の _BitScanForward() は入力が 0 のときに 0 を返す仕様なので、
	// 最初に 0 でないか判定するのは少し損。
	FORCE_INLINE Square FirstOneFromI9()
	{
		if (this->GetP(0)) {
			return FirstOneRightFromI9();
		}
		return FirstOneLeftFromB9();
	}

	// 返す位置を 0 にしないバージョン。
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

	// Bitboard の 1 の bit を数える。
	// Crossover は、MergeP() すると 1 である bit が重なる可能性があるなら true
	template <bool Crossover = true>
	int PopCount() const { return (Crossover ? count1s(GetP(0)) + count1s(GetP(1)) : count1s(MergeP())); }

	// bit が 1 つだけ立っているかどうかを判定する。
	// Crossover は、MergeP() すると 1 である bit が重なる可能性があるなら true
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

	// 指定した位置が Bitboard のどちらの u64 変数の要素か
	static int Part(const Square sq);

public://(^q^)
	inline static Bitboard SetMaskBB(const Square sq) { return g_setMaskBB[sq]; }

	// 実際に使用する部分の全て bit が立っている Bitboard
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
	// magic number を使って block の模様から利きのテーブルへのインデックスを算出
	inline u64 OccupiedToIndex( const u64 magic, const int shiftBits) const {
		return (this->MergeP() * magic) >> shiftBits;
	}
#endif

	// todo: 香車の筋がどこにあるか先に分かっていれば、Bitboard の片方の変数だけを調べれば良くなる。
	inline Bitboard LanceAttack(const Color c, const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*this).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return g_lanceAttack[c][sq][index];
	}

	// 飛車の縦だけの利き。香車の利きを使い、index を共通化することで高速化している。
	inline Bitboard RookAttackFile(const Square sq) const {
		const int part = Bitboard::Part(sq);
		const int index = ((*this).GetP(part) >> ConfigBits::m_slide[sq]) & 127;
		return g_lanceAttack[Black][sq][index] | g_lanceAttack[White][sq][index];
	}

	// Bitboard で直接利きを返す関数。
	// 1段目には歩は存在しないので、1bit シフトで別の筋に行くことはない。
	// ただし、from に歩以外の駒の Bitboard を入れると、別の筋のビットが立ってしまうことがあるので、
	// 別の筋のビットが立たないか、立っても問題ないかを確認して使用すること。
	template <Color US> inline Bitboard PawnAttack() const { // thisはfrom
		return (US == Black ? ((*this) >> 1) : ((*this) << 1));
	}

	// Bitboard で直接利きを返す関数。
	inline static Bitboard KingAttack(const Square sq) {
		return g_kingAttack[sq];
	}

	static inline Bitboard GoldAttack(const Color c, const Square sq) { return g_goldAttack[c][sq]; }
	static inline Bitboard SilverAttack(const Color c, const Square sq) { return g_silverAttack[c][sq]; }
	static inline Bitboard KnightAttack(const Color c, const Square sq) { return g_knightAttack[c][sq]; }
	static inline Bitboard PawnAttack(const Color c, const Square sq) { return g_pawnAttack[c][sq]; }


	// sq1, sq2 の間(sq1, sq2 は含まない)のビットが立った Bitboard
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
	// todo: テーブル引きを検討
	static inline Bitboard RookStepAttacks		(const Square sq						) { return Bitboard::GoldAttack(Black, sq) & Bitboard::GoldAttack(White, sq); }
	// todo: テーブル引きを検討
	static inline Bitboard BishopStepAttacks	(const Square sq						) { return Bitboard::SilverAttack(Black, sq) & Bitboard::SilverAttack(White, sq); }
	// 前方3方向の位置のBitboard
	static inline Bitboard GoldAndSilverAttacks	(const Color c		, const Square sq	) { return Bitboard::GoldAttack(c, sq) & Bitboard::SilverAttack(c, sq); }

};



