#pragma once

#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n080_common__/n080_100_common.hpp"
#include "../n120_brdEntry/n120_100_color.hpp"




const Bitboard g_FileIMask = Bitboard(UINT64_C(0x1ff) << (9 * 0), 0);
const Bitboard g_FileHMask = Bitboard(UINT64_C(0x1ff) << (9 * 1), 0);
const Bitboard g_FileGMask = Bitboard(UINT64_C(0x1ff) << (9 * 2), 0);
const Bitboard g_FileFMask = Bitboard(UINT64_C(0x1ff) << (9 * 3), 0);
const Bitboard g_FileEMask = Bitboard(UINT64_C(0x1ff) << (9 * 4), 0);
const Bitboard g_FileDMask = Bitboard(UINT64_C(0x1ff) << (9 * 5), 0);
const Bitboard g_FileCMask = Bitboard(UINT64_C(0x1ff) << (9 * 6), 0);
const Bitboard g_FileBMask = Bitboard(0, 0x1ff << (9 * 0));
const Bitboard g_FileAMask = Bitboard(0, 0x1ff << (9 * 1));

const Bitboard g_Rank9Mask = Bitboard(UINT64_C(0x40201008040201) << 0, 0x201 << 0);
const Bitboard g_Rank8Mask = Bitboard(UINT64_C(0x40201008040201) << 1, 0x201 << 1);
const Bitboard g_Rank7Mask = Bitboard(UINT64_C(0x40201008040201) << 2, 0x201 << 2);
const Bitboard g_Rank6Mask = Bitboard(UINT64_C(0x40201008040201) << 3, 0x201 << 3);
const Bitboard g_Rank5Mask = Bitboard(UINT64_C(0x40201008040201) << 4, 0x201 << 4);
const Bitboard g_Rank4Mask = Bitboard(UINT64_C(0x40201008040201) << 5, 0x201 << 5);
const Bitboard g_Rank3Mask = Bitboard(UINT64_C(0x40201008040201) << 6, 0x201 << 6);
const Bitboard g_Rank2Mask = Bitboard(UINT64_C(0x40201008040201) << 7, 0x201 << 7);
const Bitboard g_Rank1Mask = Bitboard(UINT64_C(0x40201008040201) << 8, 0x201 << 8);

extern const Bitboard g_arrFileMask[FileNum];
extern const Bitboard g_arrRankMask[RankNum];
extern const Bitboard g_arrInFrontMask[ColorNum][RankNum];

inline Bitboard fileMask(const File f) { return g_arrFileMask[f]; }
template <File F> inline Bitboard fileMask() {
	static_assert(FileI <= F && F <= FileA, "");
	return (F == FileI ? g_FileIMask
		: F == FileH ? g_FileHMask
		: F == FileG ? g_FileGMask
		: F == FileF ? g_FileFMask
		: F == FileE ? g_FileEMask
		: F == FileD ? g_FileDMask
		: F == FileC ? g_FileCMask
		: F == FileB ? g_FileBMask
		: /*F == FileA ?*/ g_FileAMask);
}

inline Bitboard rankMask(const Rank r) { return g_arrRankMask[r]; }
template <Rank R> inline Bitboard rankMask() {
	static_assert(Rank9 <= R && R <= Rank1, "");
	return (R == Rank9 ? g_Rank9Mask
		: R == Rank8 ? g_Rank8Mask
		: R == Rank7 ? g_Rank7Mask
		: R == Rank6 ? g_Rank6Mask
		: R == Rank5 ? g_Rank5Mask
		: R == Rank4 ? g_Rank4Mask
		: R == Rank3 ? g_Rank3Mask
		: R == Rank2 ? g_Rank2Mask
		: /*R == Rank1 ?*/ g_Rank1Mask);
}

// 直接テーブル引きすべきだと思う。
inline Bitboard squareFileMask(const Square sq) {
	const File f = Util_Square::MakeFile(sq);
	return fileMask(f);
}

// 直接テーブル引きすべきだと思う。
inline Bitboard squareRankMask(const Square sq) {
	const Rank r = Util_Square::MakeRank(sq);
	return rankMask(r);
}

const Bitboard g_InFrontOfRank9Black = Bitboard::allZeroBB();
const Bitboard g_InFrontOfRank8Black = rankMask<Rank9>();
const Bitboard g_InFrontOfRank7Black = g_InFrontOfRank8Black | rankMask<Rank8>();
const Bitboard g_InFrontOfRank6Black = g_InFrontOfRank7Black | rankMask<Rank7>();
const Bitboard g_InFrontOfRank5Black = g_InFrontOfRank6Black | rankMask<Rank6>();
const Bitboard g_InFrontOfRank4Black = g_InFrontOfRank5Black | rankMask<Rank5>();
const Bitboard g_InFrontOfRank3Black = g_InFrontOfRank4Black | rankMask<Rank4>();
const Bitboard g_InFrontOfRank2Black = g_InFrontOfRank3Black | rankMask<Rank3>();
const Bitboard g_InFrontOfRank1Black = g_InFrontOfRank2Black | rankMask<Rank2>();

const Bitboard g_InFrontOfRank1White = Bitboard::allZeroBB();
const Bitboard g_InFrontOfRank2White = rankMask<Rank1>();
const Bitboard g_InFrontOfRank3White = g_InFrontOfRank2White | rankMask<Rank2>();
const Bitboard g_InFrontOfRank4White = g_InFrontOfRank3White | rankMask<Rank3>();
const Bitboard g_InFrontOfRank5White = g_InFrontOfRank4White | rankMask<Rank4>();
const Bitboard g_InFrontOfRank6White = g_InFrontOfRank5White | rankMask<Rank5>();
const Bitboard g_InFrontOfRank7White = g_InFrontOfRank6White | rankMask<Rank6>();
const Bitboard g_InFrontOfRank8White = g_InFrontOfRank7White | rankMask<Rank7>();
const Bitboard g_InFrontOfRank9White = g_InFrontOfRank8White | rankMask<Rank8>();

inline Bitboard inFrontMask(const Color c, const Rank r) { return g_arrInFrontMask[c][r]; }
template <Color C, Rank R> inline Bitboard inFrontMask() {
	static_assert(C == Black || C == White, "");
	static_assert(Rank9 <= R && R <= Rank1, "");
	return (C == Black ? (R == Rank9 ? g_InFrontOfRank9Black
		: R == Rank8 ? g_InFrontOfRank8Black
		: R == Rank7 ? g_InFrontOfRank7Black
		: R == Rank6 ? g_InFrontOfRank6Black
		: R == Rank5 ? g_InFrontOfRank5Black
		: R == Rank4 ? g_InFrontOfRank4Black
		: R == Rank3 ? g_InFrontOfRank3Black
		: R == Rank2 ? g_InFrontOfRank2Black
		: /*R == Rank1 ?*/ g_InFrontOfRank1Black)
		: (R == Rank9 ? g_InFrontOfRank9White
			: R == Rank8 ? g_InFrontOfRank8White
			: R == Rank7 ? g_InFrontOfRank7White
			: R == Rank6 ? g_InFrontOfRank6White
			: R == Rank5 ? g_InFrontOfRank5White
			: R == Rank4 ? g_InFrontOfRank4White
			: R == Rank3 ? g_InFrontOfRank3White
			: R == Rank2 ? g_InFrontOfRank2White
			: /*R == Rank1 ?*/ g_InFrontOfRank1White));
}

