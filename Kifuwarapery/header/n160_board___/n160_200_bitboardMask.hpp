#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"




const Bitboard FileIMask = Bitboard(UINT64_C(0x1ff) << (9 * 0), 0);
const Bitboard FileHMask = Bitboard(UINT64_C(0x1ff) << (9 * 1), 0);
const Bitboard FileGMask = Bitboard(UINT64_C(0x1ff) << (9 * 2), 0);
const Bitboard FileFMask = Bitboard(UINT64_C(0x1ff) << (9 * 3), 0);
const Bitboard FileEMask = Bitboard(UINT64_C(0x1ff) << (9 * 4), 0);
const Bitboard FileDMask = Bitboard(UINT64_C(0x1ff) << (9 * 5), 0);
const Bitboard FileCMask = Bitboard(UINT64_C(0x1ff) << (9 * 6), 0);
const Bitboard FileBMask = Bitboard(0, 0x1ff << (9 * 0));
const Bitboard FileAMask = Bitboard(0, 0x1ff << (9 * 1));

const Bitboard Rank9Mask = Bitboard(UINT64_C(0x40201008040201) << 0, 0x201 << 0);
const Bitboard Rank8Mask = Bitboard(UINT64_C(0x40201008040201) << 1, 0x201 << 1);
const Bitboard Rank7Mask = Bitboard(UINT64_C(0x40201008040201) << 2, 0x201 << 2);
const Bitboard Rank6Mask = Bitboard(UINT64_C(0x40201008040201) << 3, 0x201 << 3);
const Bitboard Rank5Mask = Bitboard(UINT64_C(0x40201008040201) << 4, 0x201 << 4);
const Bitboard Rank4Mask = Bitboard(UINT64_C(0x40201008040201) << 5, 0x201 << 5);
const Bitboard Rank3Mask = Bitboard(UINT64_C(0x40201008040201) << 6, 0x201 << 6);
const Bitboard Rank2Mask = Bitboard(UINT64_C(0x40201008040201) << 7, 0x201 << 7);
const Bitboard Rank1Mask = Bitboard(UINT64_C(0x40201008040201) << 8, 0x201 << 8);

extern const Bitboard g_fileMask[FileNum];
extern const Bitboard g_rankMask[RankNum];
extern const Bitboard g_inFrontMask[ColorNum][RankNum];

inline Bitboard fileMask(const File f) { return g_fileMask[f]; }
template <File F> inline Bitboard fileMask() {
	static_assert(FileI <= F && F <= FileA, "");
	return (F == FileI ? FileIMask
		: F == FileH ? FileHMask
		: F == FileG ? FileGMask
		: F == FileF ? FileFMask
		: F == FileE ? FileEMask
		: F == FileD ? FileDMask
		: F == FileC ? FileCMask
		: F == FileB ? FileBMask
		: /*F == FileA ?*/ FileAMask);
}

inline Bitboard rankMask(const Rank r) { return g_rankMask[r]; }
template <Rank R> inline Bitboard rankMask() {
	static_assert(Rank9 <= R && R <= Rank1, "");
	return (R == Rank9 ? Rank9Mask
		: R == Rank8 ? Rank8Mask
		: R == Rank7 ? Rank7Mask
		: R == Rank6 ? Rank6Mask
		: R == Rank5 ? Rank5Mask
		: R == Rank4 ? Rank4Mask
		: R == Rank3 ? Rank3Mask
		: R == Rank2 ? Rank2Mask
		: /*R == Rank1 ?*/ Rank1Mask);
}

// 直接テーブル引きすべきだと思う。
inline Bitboard squareFileMask(const Square sq) {
	const File f = UtilSquare::ToFile(sq);
	return fileMask(f);
}

// 直接テーブル引きすべきだと思う。
inline Bitboard squareRankMask(const Square sq) {
	const Rank r = UtilSquare::ToRank(sq);
	return rankMask(r);
}

const Bitboard InFrontOfRank9Black = Bitboard::AllZeroBB();
const Bitboard InFrontOfRank8Black = rankMask<Rank9>();
const Bitboard InFrontOfRank7Black = InFrontOfRank8Black | rankMask<Rank8>();
const Bitboard InFrontOfRank6Black = InFrontOfRank7Black | rankMask<Rank7>();
const Bitboard InFrontOfRank5Black = InFrontOfRank6Black | rankMask<Rank6>();
const Bitboard InFrontOfRank4Black = InFrontOfRank5Black | rankMask<Rank5>();
const Bitboard InFrontOfRank3Black = InFrontOfRank4Black | rankMask<Rank4>();
const Bitboard InFrontOfRank2Black = InFrontOfRank3Black | rankMask<Rank3>();
const Bitboard InFrontOfRank1Black = InFrontOfRank2Black | rankMask<Rank2>();

const Bitboard InFrontOfRank1White = Bitboard::AllZeroBB();
const Bitboard InFrontOfRank2White = rankMask<Rank1>();
const Bitboard InFrontOfRank3White = InFrontOfRank2White | rankMask<Rank2>();
const Bitboard InFrontOfRank4White = InFrontOfRank3White | rankMask<Rank3>();
const Bitboard InFrontOfRank5White = InFrontOfRank4White | rankMask<Rank4>();
const Bitboard InFrontOfRank6White = InFrontOfRank5White | rankMask<Rank5>();
const Bitboard InFrontOfRank7White = InFrontOfRank6White | rankMask<Rank6>();
const Bitboard InFrontOfRank8White = InFrontOfRank7White | rankMask<Rank7>();
const Bitboard InFrontOfRank9White = InFrontOfRank8White | rankMask<Rank8>();

inline Bitboard inFrontMask(const Color c, const Rank r) { return g_inFrontMask[c][r]; }
template <Color C, Rank R> inline Bitboard inFrontMask() {
	static_assert(C == Black || C == White, "");
	static_assert(Rank9 <= R && R <= Rank1, "");
	return (C == Black ? (R == Rank9 ? InFrontOfRank9Black
		: R == Rank8 ? InFrontOfRank8Black
		: R == Rank7 ? InFrontOfRank7Black
		: R == Rank6 ? InFrontOfRank6Black
		: R == Rank5 ? InFrontOfRank5Black
		: R == Rank4 ? InFrontOfRank4Black
		: R == Rank3 ? InFrontOfRank3Black
		: R == Rank2 ? InFrontOfRank2Black
		: /*R == Rank1 ?*/ InFrontOfRank1Black)
		: (R == Rank9 ? InFrontOfRank9White
			: R == Rank8 ? InFrontOfRank8White
			: R == Rank7 ? InFrontOfRank7White
			: R == Rank6 ? InFrontOfRank6White
			: R == Rank5 ? InFrontOfRank5White
			: R == Rank4 ? InFrontOfRank4White
			: R == Rank3 ? InFrontOfRank3White
			: R == Rank2 ? InFrontOfRank2White
			: /*R == Rank1 ?*/ InFrontOfRank1White));
}

