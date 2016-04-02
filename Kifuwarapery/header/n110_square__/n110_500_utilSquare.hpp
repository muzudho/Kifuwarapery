#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_300_direction.hpp"


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

	static inline bool isInFile(const File f) { return (0 <= f) && (f < FileNum); }
	static inline bool isInRank(const Rank r) { return (0 <= r) && (r < RankNum); }
	// s が Square の中に含まれているか判定
	static inline bool isInSquare(const Square s) { return (0 <= s) && (s < SquareNum); }
	// File, Rank のどちらかがおかしいかも知れない時は、
	// こちらを使う。
	// こちらの方が遅いが、どんな File, Rank にも対応している。
	static inline bool isInSquare(const File f, const Rank r) { return isInFile(f) && isInRank(r); }

	// 速度が必要な場面で使用するなら、テーブル引きの方が有効だと思う。
	static inline constexpr Square makeSquare(const File f, const Rank r) {
		return static_cast<Square>(static_cast<int>(f) * 9 + static_cast<int>(r));
	}

	// 速度が必要な場面で使用する。
	static inline Rank makeRank(const Square s) {
		assert(isInSquare(s));
		return SquareToRank[s];
	}
	static inline File makeFile(const Square s) {
		assert(isInSquare(s));
		return SquareToFile[s];
	}

};

static inline Square operator + (const Square lhs, const SquareDelta rhs) { return lhs + static_cast<Square>(rhs); }
static inline void operator += (Square& lhs, const SquareDelta rhs) { lhs = lhs + static_cast<Square>(rhs); }
static inline Square operator - (const Square lhs, const SquareDelta rhs) { return lhs - static_cast<Square>(rhs); }
static inline void operator -= (Square& lhs, const SquareDelta rhs) { lhs = lhs - static_cast<Square>(rhs); }





// 2つの位置関係のテーブル
extern Direction SquareRelation[SquareNum][SquareNum];
inline Direction squareRelation(const Square sq1, const Square sq2) { return SquareRelation[sq1][sq2]; }

// 何かの駒で一手で行ける位置関係についての距離のテーブル。桂馬の位置は距離1とする。
extern int SquareDistance[SquareNum][SquareNum];
inline int squareDistance(const Square sq1, const Square sq2) { return SquareDistance[sq1][sq2]; }

// from, to, ksq が 縦横斜めの同一ライン上にあれば true を返す。
template <bool FROM_KSQ_NEVER_BE_DIRECMISC>
inline bool isAligned(const Square from, const Square to, const Square ksq) {
	const Direction direc = squareRelation(from, ksq);
	if (FROM_KSQ_NEVER_BE_DIRECMISC) {
		assert(direc != DirecMisc);
		return (direc == squareRelation(from, to));
	}
	else {
		return (direc != DirecMisc && direc == squareRelation(from, to));
	}
}

inline char fileToCharUSI(const File f) { return '1' + f; }
// todo: アルファベットが辞書順に並んでいない処理系があるなら対応すること。
inline char rankToCharUSI(const Rank r) {
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
inline std::string squareToStringUSI(const Square sq) {
	const Rank r = UtilSquare::makeRank(sq);
	const File f = UtilSquare::makeFile(sq);
	const char ch[] = {fileToCharUSI(f), rankToCharUSI(r), '\0'};
	return std::string(ch);
}

inline char fileToCharCSA(const File f) { return '1' + f; }
inline char rankToCharCSA(const Rank r) { return '1' + r; }
inline std::string squareToStringCSA(const Square sq) {
	const Rank r = UtilSquare::makeRank(sq);
	const File f = UtilSquare::makeFile(sq);
	const char ch[] = {fileToCharCSA(f), rankToCharCSA(r), '\0'};
	return std::string(ch);
}

inline File charCSAToFile(const char c) { return static_cast<File>(c - '1'); }
inline Rank charCSAToRank(const char c) { return static_cast<Rank>(c - '1'); }
inline File charUSIToFile(const char c) { return static_cast<File>(c - '1'); }
inline Rank charUSIToRank(const char c) { return static_cast<Rank>(c - 'a'); }

// 後手の位置を先手の位置へ変換
inline constexpr Square inverse(const Square sq) { return SquareNum - 1 - sq; }
// 左右変換
inline constexpr File inverse(const File f) { return FileNum - 1 - f; }
// 上下変換
inline constexpr Rank inverse(const Rank r) { return RankNum - 1 - r; }
// Square の左右だけ変換
inline Square inverseFile(const Square sq) { return UtilSquare::makeSquare(inverse(UtilSquare::makeFile(sq)), UtilSquare::makeRank(sq)); }

inline constexpr Square inverseIfWhite(const Color c, const Square sq) { return (c == Black ? sq : inverse(sq)); }

inline bool canPromote(const Color c, const Rank fromOrToRank) {
#if 1
	static_assert(Black == 0, "");
	static_assert(Rank9 == 0, "");
	return static_cast<bool>(0x1c00007u & (1u << ((c << 4) + fromOrToRank)));
#else
	// 同じ意味。
	return (c == Black ? IsInFrontOf<Black, Rank6, Rank4>(fromOrToRank) : IsInFrontOf<White, Rank6, Rank4>(fromOrToRank));
#endif
}
