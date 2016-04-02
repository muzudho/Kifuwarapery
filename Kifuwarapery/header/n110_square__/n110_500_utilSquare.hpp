#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_300_direction.hpp"

// 2つの位置関係のテーブル
extern Direction g_squareRelation[SquareNum][SquareNum];
// 何かの駒で一手で行ける位置関係についての距離のテーブル。桂馬の位置は距離1とする。
extern int g_squareDistance[SquareNum][SquareNum];

class UtilSquare {
public:

	// 先手のときは BRANK, 後手のときは WRANK より target が前の段にあるなら true を返す。
	template <Color US, Rank BRANK, Rank WRANK>
	static inline bool IsInFrontOf(const Rank target) { return (US == Black ? (target < BRANK) : (WRANK < target)); }

	template <Color US, Rank BRANK, Rank WRANK>
	static inline bool IsBehind(const Rank target) { return (US == Black ? (BRANK < target) : (target < WRANK)); }

	template <Color US, File BFILE, File WFILE>
	static inline bool IsLeftOf(const File target) { return (US == Black ? (BFILE < target) : (target < WFILE)); }

	template <Color US, File BFILE, File WFILE>
	static inline bool IsRightOf(const File target) { return (US == Black ? (target < BFILE) : (WFILE < target)); }

	static inline bool IsInFile(const File f) { return (0 <= f) && (f < FileNum); }
	static inline bool IsInRank(const Rank r) { return (0 <= r) && (r < RankNum); }
	// s が Square の中に含まれているか判定
	static inline bool IsInSquare(const Square s) { return (0 <= s) && (s < SquareNum); }
	// File, Rank のどちらかがおかしいかも知れない時は、
	// こちらを使う。
	// こちらの方が遅いが、どんな File, Rank にも対応している。
	static inline bool IsInSquare(const File f, const Rank r) { return IsInFile(f) && IsInRank(r); }

	// 速度が必要な場面で使用するなら、テーブル引きの方が有効だと思う。
	static inline constexpr Square MakeSquare(const File f, const Rank r) {
		return static_cast<Square>(static_cast<int>(f) * 9 + static_cast<int>(r));
	}

	// 速度が必要な場面で使用する。
	static inline Rank MakeRank(const Square s) {
		assert(UtilSquare::IsInSquare(s));
		return SquareToRank[s];
	}
	static inline File MakeFile(const Square s) {
		assert(UtilSquare::IsInSquare(s));
		return SquareToFile[s];
	}

	static inline Direction GetSquareRelation(const Square sq1, const Square sq2) { return g_squareRelation[sq1][sq2]; }

	static inline int GetSquareDistance(const Square sq1, const Square sq2) { return g_squareDistance[sq1][sq2]; }

	// from, to, ksq が 縦横斜めの同一ライン上にあれば true を返す。
	template <bool FROM_KSQ_NEVER_BE_DIRECMISC>
	static inline bool IsAligned(const Square from, const Square to, const Square ksq) {
		const Direction direc = UtilSquare::GetSquareRelation(from, ksq);
		if (FROM_KSQ_NEVER_BE_DIRECMISC) {
			assert(direc != DirecMisc);
			return (direc == UtilSquare::GetSquareRelation(from, to));
		}
		else {
			return (direc != DirecMisc && direc == UtilSquare::GetSquareRelation(from, to));
		}
	}

	static inline char FileToCharUSI(const File f) { return '1' + f; }

	// todo: アルファベットが辞書順に並んでいない処理系があるなら対応すること。
	static inline char RankToCharUSI(const Rank r) {
		static_assert('a' + 1 == 'b', "");
		static_assert('a' + 2 == 'c', "");
		static_assert('a' + 3 == 'd', "");
		static_assert('a' + 4 == 'e', "");
		static_assert('a' + 5 == 'f', "");
		static_assert('a' + 6 == 'g', "");
		static_assert('a' + 7 == 'h', "");
		static_assert('a' + 8 == 'i', "");
		return 'a' + r;
	}
	static inline std::string SquareToStringUSI(const Square sq) {
		const Rank r = UtilSquare::MakeRank(sq);
		const File f = UtilSquare::MakeFile(sq);
		const char ch[] = { UtilSquare::FileToCharUSI(f), UtilSquare::RankToCharUSI(r), '\0' };
		return std::string(ch);
	}


	static inline char FileToCharCSA(const File f) { return '1' + f; }
	static inline char RankToCharCSA(const Rank r) { return '1' + r; }
	static inline std::string squareToStringCSA(const Square sq) {
		const Rank r = UtilSquare::MakeRank(sq);
		const File f = UtilSquare::MakeFile(sq);
		const char ch[] = { FileToCharCSA(f), RankToCharCSA(r), '\0' };
		return std::string(ch);
	}

	static inline File CharCSAToFile(const char c) { return static_cast<File>(c - '1'); }
	static inline Rank CharCSAToRank(const char c) { return static_cast<Rank>(c - '1'); }
	static inline File CharUSIToFile(const char c) { return static_cast<File>(c - '1'); }
	static inline Rank CharUSIToRank(const char c) { return static_cast<Rank>(c - 'a'); }

	// 後手の位置を先手の位置へ変換
	static inline constexpr Square Inverse(const Square sq) { return SquareNum - 1 - sq; }
	// 左右変換
	static inline constexpr File Inverse(const File f) { return FileNum - 1 - f; }
	// 上下変換
	static inline constexpr Rank Inverse(const Rank r) { return RankNum - 1 - r; }
	// Square の左右だけ変換
	static inline Square InverseFile(const Square sq) { return UtilSquare::MakeSquare(Inverse(UtilSquare::MakeFile(sq)), UtilSquare::MakeRank(sq)); }

	static inline constexpr Square InverseIfWhite(const Color c, const Square sq) { return (c == Black ? sq : Inverse(sq)); }

	static inline bool CanPromote(const Color c, const Rank fromOrToRank) {
#if 1
		static_assert(Black == 0, "");
		static_assert(Rank9 == 0, "");
		return static_cast<bool>(0x1c00007u & (1u << ((c << 4) + fromOrToRank)));
#else
		// 同じ意味。
		return (c == Black ? IsInFrontOf<Black, Rank6, Rank4>(fromOrToRank) : IsInFrontOf<White, Rank6, Rank4>(fromOrToRank));
#endif
	}

};

static inline Square operator + (const Square lhs, const SquareDelta rhs) { return lhs + static_cast<Square>(rhs); }
static inline void operator += (Square& lhs, const SquareDelta rhs) { lhs = lhs + static_cast<Square>(rhs); }
static inline Square operator - (const Square lhs, const SquareDelta rhs) { return lhs - static_cast<Square>(rhs); }
static inline void operator -= (Square& lhs, const SquareDelta rhs) { lhs = lhs - static_cast<Square>(rhs); }



