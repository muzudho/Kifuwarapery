#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_460_RankMaskBb.hpp"



extern const RankMaskBb g_rankMaskBb;
extern const Bitboard g_inFrontMask[ColorNum][RankNum];



class BitboardMask {
public:

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
const Bitboard g_InFrontOfRank8Black = g_rankMaskBb.GetRankMask<Rank9>();
const Bitboard g_InFrontOfRank7Black = g_InFrontOfRank8Black | g_rankMaskBb.GetRankMask<Rank8>();
const Bitboard g_InFrontOfRank6Black = g_InFrontOfRank7Black | g_rankMaskBb.GetRankMask<Rank7>();
const Bitboard g_InFrontOfRank5Black = g_InFrontOfRank6Black | g_rankMaskBb.GetRankMask<Rank6>();
const Bitboard g_InFrontOfRank4Black = g_InFrontOfRank5Black | g_rankMaskBb.GetRankMask<Rank5>();
const Bitboard g_InFrontOfRank3Black = g_InFrontOfRank4Black | g_rankMaskBb.GetRankMask<Rank4>();
const Bitboard g_InFrontOfRank2Black = g_InFrontOfRank3Black | g_rankMaskBb.GetRankMask<Rank3>();
const Bitboard g_InFrontOfRank1Black = g_InFrontOfRank2Black | g_rankMaskBb.GetRankMask<Rank2>();

const Bitboard g_InFrontOfRank1White = Bitboard::CreateAllZeroBB();
const Bitboard g_InFrontOfRank2White = g_rankMaskBb.GetRankMask<Rank1>();
const Bitboard g_InFrontOfRank3White = g_InFrontOfRank2White | g_rankMaskBb.GetRankMask<Rank2>();
const Bitboard g_InFrontOfRank4White = g_InFrontOfRank3White | g_rankMaskBb.GetRankMask<Rank3>();
const Bitboard g_InFrontOfRank5White = g_InFrontOfRank4White | g_rankMaskBb.GetRankMask<Rank4>();
const Bitboard g_InFrontOfRank6White = g_InFrontOfRank5White | g_rankMaskBb.GetRankMask<Rank5>();
const Bitboard g_InFrontOfRank7White = g_InFrontOfRank6White | g_rankMaskBb.GetRankMask<Rank6>();
const Bitboard g_InFrontOfRank8White = g_InFrontOfRank7White | g_rankMaskBb.GetRankMask<Rank7>();
const Bitboard g_InFrontOfRank9White = g_InFrontOfRank8White | g_rankMaskBb.GetRankMask<Rank8>();
