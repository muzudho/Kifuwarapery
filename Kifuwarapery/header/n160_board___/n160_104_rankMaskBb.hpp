#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"


class RankMaskBb {
public:
	const Bitboard m_9 = Bitboard(UINT64_C(0x40201008040201) << 0, 0x201 << 0);
	const Bitboard m_8 = Bitboard(UINT64_C(0x40201008040201) << 1, 0x201 << 1);
	const Bitboard m_7 = Bitboard(UINT64_C(0x40201008040201) << 2, 0x201 << 2);
	const Bitboard m_6 = Bitboard(UINT64_C(0x40201008040201) << 3, 0x201 << 3);
	const Bitboard m_5 = Bitboard(UINT64_C(0x40201008040201) << 4, 0x201 << 4);
	const Bitboard m_4 = Bitboard(UINT64_C(0x40201008040201) << 5, 0x201 << 5);
	const Bitboard m_3 = Bitboard(UINT64_C(0x40201008040201) << 6, 0x201 << 6);
	const Bitboard m_2 = Bitboard(UINT64_C(0x40201008040201) << 7, 0x201 << 7);
	const Bitboard m_1 = Bitboard(UINT64_C(0x40201008040201) << 8, 0x201 << 8);

	const Bitboard m_rankMask[RankNum] = {
		this->m_9,
		this->m_8,
		this->m_7,
		this->m_6,
		this->m_5,
		this->m_4,
		this->m_3,
		this->m_2,
		this->m_1
	};

public:
	inline Bitboard GetRankMask(const Rank r) const {
		return this->m_rankMask[r];
	}

	template <Rank R>
	inline Bitboard GetRankMask() const {
		static_assert(Rank9 <= R && R <= Rank1, "");
		return (R == Rank9 ? this->m_9
			: R == Rank8 ? this->m_8
			: R == Rank7 ? this->m_7
			: R == Rank6 ? this->m_6
			: R == Rank5 ? this->m_5
			: R == Rank4 ? this->m_4
			: R == Rank3 ? this->m_3
			: R == Rank2 ? this->m_2
			: /*R == Rank1 ?*/ this->m_1);
	}

	// 直接テーブル引きすべきだと思う。
	inline Bitboard GetSquareRankMask(const Square sq) const {
		const Rank r = UtilSquare::ToRank(sq);
		return this->GetRankMask(r);
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern const RankMaskBb g_rankMaskBb;
