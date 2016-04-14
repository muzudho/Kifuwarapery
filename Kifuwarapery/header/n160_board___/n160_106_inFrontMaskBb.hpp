#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_200_rank.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_104_RankMaskBb.hpp"



extern const RankMaskBb g_rankMaskBb;



class InFrontMaskBb {
public:
	// g_InFrontOfRank9Black...
	const Bitboard m_ofRank9Black = Bitboard::CreateAllZeroBB();
	const Bitboard m_ofRank8Black = g_rankMaskBb.GetRankMask_rank(Rank9);
	const Bitboard m_ofRank7Black = m_ofRank8Black | g_rankMaskBb.GetRankMask_rank(Rank8);
	const Bitboard m_ofRank6Black = m_ofRank7Black | g_rankMaskBb.GetRankMask_rank(Rank7);
	const Bitboard m_ofRank5Black = m_ofRank6Black | g_rankMaskBb.GetRankMask_rank(Rank6);
	const Bitboard m_ofRank4Black = m_ofRank5Black | g_rankMaskBb.GetRankMask_rank(Rank5);
	const Bitboard m_ofRank3Black = m_ofRank4Black | g_rankMaskBb.GetRankMask_rank(Rank4);
	const Bitboard m_ofRank2Black = m_ofRank3Black | g_rankMaskBb.GetRankMask_rank(Rank3);
	const Bitboard m_ofRank1Black = m_ofRank2Black | g_rankMaskBb.GetRankMask_rank(Rank2);

	const Bitboard m_ofRank1White = Bitboard::CreateAllZeroBB();
	const Bitboard m_ofRank2White = g_rankMaskBb.GetRankMask_rank(Rank1);
	const Bitboard m_ofRank3White = m_ofRank2White | g_rankMaskBb.GetRankMask_rank(Rank2);
	const Bitboard m_ofRank4White = m_ofRank3White | g_rankMaskBb.GetRankMask_rank(Rank3);
	const Bitboard m_ofRank5White = m_ofRank4White | g_rankMaskBb.GetRankMask_rank(Rank4);
	const Bitboard m_ofRank6White = m_ofRank5White | g_rankMaskBb.GetRankMask_rank(Rank5);
	const Bitboard m_ofRank7White = m_ofRank6White | g_rankMaskBb.GetRankMask_rank(Rank6);
	const Bitboard m_ofRank8White = m_ofRank7White | g_rankMaskBb.GetRankMask_rank(Rank7);
	const Bitboard m_ofRank9White = m_ofRank8White | g_rankMaskBb.GetRankMask_rank(Rank8);


	const Bitboard m_inFrontMask[ColorNum][RankNum] = {
		{
			this->m_ofRank9Black,
			this->m_ofRank8Black,
			this->m_ofRank7Black,
			this->m_ofRank6Black,
			this->m_ofRank5Black,
			this->m_ofRank4Black,
			this->m_ofRank3Black,
			this->m_ofRank2Black,
			this->m_ofRank1Black
		},
		{
			this->m_ofRank9White,
			this->m_ofRank8White,
			this->m_ofRank7White,
			this->m_ofRank6White,
			this->m_ofRank5White,
			this->m_ofRank4White,
			this->m_ofRank3White,
			this->m_ofRank2White,
			this->m_ofRank1White
		}
	};

public:

	inline Bitboard GetInFrontMask(const Color c, const Rank r) const {
		return this->m_inFrontMask[c][r];
	}

	template <Color C, Rank R>
	inline Bitboard GetInFrontMask() const {
		static_assert(C == Black || C == White, "");
		static_assert(Rank9 <= R && R <= Rank1, "");
		return (C == Black ? (R == Rank9 ? this->m_ofRank9Black
			: R == Rank8 ? this->m_ofRank8Black
			: R == Rank7 ? this->m_ofRank7Black
			: R == Rank6 ? this->m_ofRank6Black
			: R == Rank5 ? this->m_ofRank5Black
			: R == Rank4 ? this->m_ofRank4Black
			: R == Rank3 ? this->m_ofRank3Black
			: R == Rank2 ? this->m_ofRank2Black
			: /*R == Rank1 ?*/ this->m_ofRank1Black)
			: (R == Rank9 ? this->m_ofRank9White
				: R == Rank8 ? this->m_ofRank8White
				: R == Rank7 ? this->m_ofRank7White
				: R == Rank6 ? this->m_ofRank6White
				: R == Rank5 ? this->m_ofRank5White
				: R == Rank4 ? this->m_ofRank4White
				: R == Rank3 ? this->m_ofRank3White
				: R == Rank2 ? this->m_ofRank2White
				: /*R == Rank1 ?*/ this->m_ofRank1White));
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern const InFrontMaskBb g_inFrontMaskBb;