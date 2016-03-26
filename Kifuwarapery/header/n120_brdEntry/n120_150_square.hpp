#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"
#include "../n080_common__/n080_100_common.hpp"
#include "../n120_brdEntry/n120_100_color.hpp"

// 盤面を [0, 80] の整数の index で表す
// I9 = 1一, I1 = 1九, A1 = 9九
//
// A9, B9, C9, D9, E9, F9, G9, H9, I9,
// A8, B8, C8, D8, E8, F8, G8, H8, I8,
// A7, B7, C7, D7, E7, F7, G7, H7, I7,
// A6, B6, C6, D6, E6, F6, G6, H6, I6,
// A5, B5, C5, D5, E5, F5, G5, H5, I5,
// A4, B4, C4, D4, E4, F4, G4, H4, I4,
// A3, B3, C3, D3, E3, F3, G3, H3, I3,
// A2, B2, C2, D2, E2, F2, G2, H2, I2,
// A1, B1, C1, D1, E1, F1, G1, H1, I1,
//
// Bitboard のビットが縦に並んでいて、
// 0 ビット目から順に、以下の位置と対応させる。
enum Square {
	I9, I8, I7, I6, I5, I4, I3, I2, I1,
	H9,	H8, H7, H6, H5, H4, H3, H2, H1,
	G9,	G8, G7, G6, G5, G4, G3, G2, G1,
	F9,	F8, F7, F6, F5, F4, F3, F2, F1,
	E9,	E8, E7, E6, E5, E4, E3, E2, E1,
	D9,	D8, D7, D6, D5, D4, D3, D2, D1,
	C9,	C8, C7, C6, C5, C4, C3, C2, C1,
	B9,	B8, B7, B6, B5, B4, B3, B2, B1,
	A9,	A8, A7, A6, A5, A4, A3, A2, A1,
	SquareNum, // = 81
	SquareNoLeftNum = D9,
	B_hand_pawn   = SquareNum     + -1,
	B_hand_lance  = B_hand_pawn   + 18,
	B_hand_knight = B_hand_lance  +  4,
	B_hand_silver = B_hand_knight +  4,
	B_hand_gold   = B_hand_silver +  4,
	B_hand_bishop = B_hand_gold   +  4,
	B_hand_rook   = B_hand_bishop +  2,
	W_hand_pawn   = B_hand_rook   +  2,
	W_hand_lance  = W_hand_pawn   + 18,
	W_hand_knight = W_hand_lance  +  4,
	W_hand_silver = W_hand_knight +  4,
	W_hand_gold   = W_hand_silver +  4,
	W_hand_bishop = W_hand_gold   +  4,
	W_hand_rook   = W_hand_bishop +  2,
	SquareHandNum = W_hand_rook   +  3
};
OverloadEnumOperators(Square);

enum File {
	FileI, FileH, FileG, FileF, FileE, FileD, FileC, FileB, FileA, FileNum,
	FileNoLeftNum = FileD
};
OverloadEnumOperators(File);

enum Rank {
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1, RankNum
};
OverloadEnumOperators(Rank);


const Rank SquareToRank[SquareNum] = {
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1
};

const File SquareToFile[SquareNum] = {
	FileI, FileI, FileI, FileI, FileI, FileI, FileI, FileI, FileI,
	FileH, FileH, FileH, FileH, FileH, FileH, FileH, FileH, FileH,
	FileG, FileG, FileG, FileG, FileG, FileG, FileG, FileG, FileG,
	FileF, FileF, FileF, FileF, FileF, FileF, FileF, FileF, FileF,
	FileE, FileE, FileE, FileE, FileE, FileE, FileE, FileE, FileE,
	FileD, FileD, FileD, FileD, FileD, FileD, FileD, FileD, FileD,
	FileC, FileC, FileC, FileC, FileC, FileC, FileC, FileC, FileC,
	FileB, FileB, FileB, FileB, FileB, FileB, FileB, FileB, FileB,
	FileA, FileA, FileA, FileA, FileA, FileA, FileA, FileA, FileA
};



enum SquareDelta {
	DeltaNothing = 0, // 同一の Square にあるとき
	DeltaN = -1, DeltaE = -9, DeltaS = 1, DeltaW = 9,
	DeltaNE = DeltaN + DeltaE,
	DeltaSE = DeltaS + DeltaE,
	DeltaSW = DeltaS + DeltaW,
	DeltaNW = DeltaN + DeltaW
};
OverloadEnumOperators(SquareDelta);

inline Square operator + (const Square lhs, const SquareDelta rhs) { return lhs + static_cast<Square>(rhs); }
inline void operator += (Square& lhs, const SquareDelta rhs) { lhs = lhs + static_cast<Square>(rhs); }
inline Square operator - (const Square lhs, const SquareDelta rhs) { return lhs - static_cast<Square>(rhs); }
inline void operator -= (Square& lhs, const SquareDelta rhs) { lhs = lhs - static_cast<Square>(rhs); }



// 位置関係、方向
// ボナンザそのまま
// でもあまり使わないので普通の enum と同様に 0 から順に値を付けて行けば良いと思う。
enum Direction {
	DirecMisc = Binary<  0>::value, // 縦、横、斜めの位置に無い場合
	DirecFile = Binary< 10>::value, // 縦
	DirecRank = Binary< 11>::value, // 横
	DirecDiagNESW = Binary<100>::value, // 右上から左下
	DirecDiagNWSE = Binary<101>::value, // 左上から右下
	DirecCross = Binary< 10>::value, // 縦、横
	DirecDiag = Binary<100>::value, // 斜め
};
OverloadEnumOperators(Direction);




// 2つの位置関係のテーブル
extern Direction SquareRelation[SquareNum][SquareNum];
inline Direction squareRelation(const Square sq1, const Square sq2) { return SquareRelation[sq1][sq2]; }

// 何かの駒で一手で行ける位置関係についての距離のテーブル。桂馬の位置は距離1とする。
extern int SquareDistance[SquareNum][SquareNum];
inline int squareDistance(const Square sq1, const Square sq2) { return SquareDistance[sq1][sq2]; }




class Util_Square {
public:
	// 先手のときは BRANK, 後手のときは WRANK より target が前の段にあるなら true を返す。
	template <Color US, Rank BRANK, Rank WRANK>
	inline static bool IsInFrontOf(const Rank target) { return (US == Black ? (target < BRANK) : (WRANK < target)); }

	template <Color US, Rank BRANK, Rank WRANK>
	inline static bool IsBehind(const Rank target) { return (US == Black ? (BRANK < target) : (target < WRANK)); }

	template <Color US, File BFILE, File WFILE>
	inline static bool IsLeftOf(const File target) { return (US == Black ? (BFILE < target) : (target < WFILE)); }

	template <Color US, File BFILE, File WFILE>
	inline static bool IsRightOf(const File target) { return (US == Black ? (target < BFILE) : (WFILE < target)); }

	inline static bool IsInFile(const File f) { return (0 <= f) && (f < FileNum); }
	inline static bool IsInRank(const Rank r) { return (0 <= r) && (r < RankNum); }
	// s が Square の中に含まれているか判定
	inline static bool IsInSquare(const Square s) { return (0 <= s) && (s < SquareNum); }
	// File, Rank のどちらかがおかしいかも知れない時は、
	// こちらを使う。
	// こちらの方が遅いが、どんな File, Rank にも対応している。
	inline static bool IsInSquare(const File f, const Rank r) { return Util_Square::IsInFile(f) && Util_Square::IsInRank(r); }

	// 速度が必要な場面で使用する。
	inline static Rank MakeRank(const Square s) {
		assert(Util_Square::IsInSquare(s));
		return SquareToRank[s];
	}
	inline static File MakeFile(const Square s) {
		assert(Util_Square::IsInSquare(s));
		return SquareToFile[s];
	}

	// 速度が必要な場面で使用するなら、テーブル引きの方が有効だと思う。
	inline static constexpr Square MakeSquare(const File f, const Rank r) {
		return static_cast<Square>(static_cast<int>(f) * 9 + static_cast<int>(r));
	}


	// from, to, ksq が 縦横斜めの同一ライン上にあれば true を返す。
	template <bool FROM_KSQ_NEVER_BE_DIRECMISC>
	inline static bool IsAligned(const Square from, const Square to, const Square ksq) {
		const Direction direc = squareRelation(from, ksq);
		if (FROM_KSQ_NEVER_BE_DIRECMISC) {
			assert(direc != DirecMisc);
			return (direc == squareRelation(from, to));
		}
		else {
			return (direc != DirecMisc && direc == squareRelation(from, to));
		}
	}

	inline static char FileToCharUSI(const File f) { return '1' + f; }

	// todo: アルファベットが辞書順に並んでいない処理系があるなら対応すること。
	inline static char RankToCharUSI(const Rank r) {
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

	inline static std::string SquareToStringUSI(const Square sq) {
		const Rank r = Util_Square::MakeRank(sq);
		const File f = Util_Square::MakeFile(sq);
		const char ch[] = { Util_Square::FileToCharUSI(f), Util_Square::RankToCharUSI(r), '\0' };
		return std::string(ch);
	}

	inline static char FileToCharCSA(const File f) { return '1' + f; }
	inline static char RankToCharCSA(const Rank r) { return '1' + r; }
	inline static std::string SquareToStringCSA(const Square sq) {
		const Rank r = Util_Square::MakeRank(sq);
		const File f = Util_Square::MakeFile(sq);
		const char ch[] = { Util_Square::FileToCharCSA(f), Util_Square::RankToCharCSA(r), '\0' };
		return std::string(ch);
	}

	inline static File CharCSAToFile(const char c) { return static_cast<File>(c - '1'); }
	inline static Rank CharCSAToRank(const char c) { return static_cast<Rank>(c - '1'); }
	inline static File CharUSIToFile(const char c) { return static_cast<File>(c - '1'); }
	inline static Rank CharUSIToRank(const char c) { return static_cast<Rank>(c - 'a'); }


	// 後手の位置を先手の位置へ変換
	inline static constexpr Square Inverse(const Square sq) { return SquareNum - 1 - sq; }
	// 左右変換
	inline static constexpr File Inverse(const File f) { return FileNum - 1 - f; }
	// 上下変換
	inline static constexpr Rank Inverse(const Rank r) { return RankNum - 1 - r; }
	// Square の左右だけ変換
	inline static Square InverseFile(const Square sq) {
		return Util_Square::MakeSquare(Util_Square::Inverse(Util_Square::MakeFile(sq)), Util_Square::MakeRank(sq)); }

	inline static constexpr Square InverseIfWhite(const Color c, const Square sq) {
		return (c == Black ? sq : Util_Square::Inverse(sq)); }

	inline static bool CanPromote(const Color c, const Rank fromOrToRank) {
#if 1
		static_assert(Black == 0, "");
		static_assert(Rank9 == 0, "");
		return static_cast<bool>(0x1c00007u & (1u << ((c << 4) + fromOrToRank)));
#else
		// 同じ意味。
		return (c == Black ? isInFrontOf<Black, Rank6, Rank4>(fromOrToRank) : isInFrontOf<White, Rank6, Rank4>(fromOrToRank));
#endif
	}
};





