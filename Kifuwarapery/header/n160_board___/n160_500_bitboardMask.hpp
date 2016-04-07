#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_450_FileMaskBb.hpp"



const Bitboard g_Rank9Mask = Bitboard(UINT64_C(0x40201008040201) << 0, 0x201 << 0);
const Bitboard g_Rank8Mask = Bitboard(UINT64_C(0x40201008040201) << 1, 0x201 << 1);
const Bitboard g_Rank7Mask = Bitboard(UINT64_C(0x40201008040201) << 2, 0x201 << 2);
const Bitboard g_Rank6Mask = Bitboard(UINT64_C(0x40201008040201) << 3, 0x201 << 3);
const Bitboard g_Rank5Mask = Bitboard(UINT64_C(0x40201008040201) << 4, 0x201 << 4);
const Bitboard g_Rank4Mask = Bitboard(UINT64_C(0x40201008040201) << 5, 0x201 << 5);
const Bitboard g_Rank3Mask = Bitboard(UINT64_C(0x40201008040201) << 6, 0x201 << 6);
const Bitboard g_Rank2Mask = Bitboard(UINT64_C(0x40201008040201) << 7, 0x201 << 7);
const Bitboard g_Rank1Mask = Bitboard(UINT64_C(0x40201008040201) << 8, 0x201 << 8);

//extern const FileMaskBb g_fileMaskBb;
extern const Bitboard g_rankMask[RankNum];
extern const Bitboard g_inFrontMask[ColorNum][RankNum];



class BitboardMask {
public:

	static inline Bitboard GetRankMask(const Rank r) { return g_rankMask[r]; }

	template <Rank R>
	static inline Bitboard GetRankMask() {
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
	static inline Bitboard GetSquareRankMask(const Square sq) {
		const Rank r = UtilSquare::ToRank(sq);
		return BitboardMask::GetRankMask(r);
	}

	static inline Bitboard GetInFrontMask(const Color c, const Rank r) { return g_inFrontMask[c][r]; }

	template <Color C, Rank R>
	static inline Bitboard GetInFrontMask() {
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

};


const Bitboard g_InFrontOfRank9Black = Bitboard::CreateAllZeroBB();
const Bitboard g_InFrontOfRank8Black = BitboardMask::GetRankMask<Rank9>();
const Bitboard g_InFrontOfRank7Black = g_InFrontOfRank8Black | BitboardMask::GetRankMask<Rank8>();
const Bitboard g_InFrontOfRank6Black = g_InFrontOfRank7Black | BitboardMask::GetRankMask<Rank7>();
const Bitboard g_InFrontOfRank5Black = g_InFrontOfRank6Black | BitboardMask::GetRankMask<Rank6>();
const Bitboard g_InFrontOfRank4Black = g_InFrontOfRank5Black | BitboardMask::GetRankMask<Rank5>();
const Bitboard g_InFrontOfRank3Black = g_InFrontOfRank4Black | BitboardMask::GetRankMask<Rank4>();
const Bitboard g_InFrontOfRank2Black = g_InFrontOfRank3Black | BitboardMask::GetRankMask<Rank3>();
const Bitboard g_InFrontOfRank1Black = g_InFrontOfRank2Black | BitboardMask::GetRankMask<Rank2>();

const Bitboard g_InFrontOfRank1White = Bitboard::CreateAllZeroBB();
const Bitboard g_InFrontOfRank2White = BitboardMask::GetRankMask<Rank1>();
const Bitboard g_InFrontOfRank3White = g_InFrontOfRank2White | BitboardMask::GetRankMask<Rank2>();
const Bitboard g_InFrontOfRank4White = g_InFrontOfRank3White | BitboardMask::GetRankMask<Rank3>();
const Bitboard g_InFrontOfRank5White = g_InFrontOfRank4White | BitboardMask::GetRankMask<Rank4>();
const Bitboard g_InFrontOfRank6White = g_InFrontOfRank5White | BitboardMask::GetRankMask<Rank5>();
const Bitboard g_InFrontOfRank7White = g_InFrontOfRank6White | BitboardMask::GetRankMask<Rank6>();
const Bitboard g_InFrontOfRank8White = g_InFrontOfRank7White | BitboardMask::GetRankMask<Rank7>();
const Bitboard g_InFrontOfRank9White = g_InFrontOfRank8White | BitboardMask::GetRankMask<Rank8>();
