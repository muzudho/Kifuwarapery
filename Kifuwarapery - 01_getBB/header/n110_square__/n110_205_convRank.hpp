#pragma once

#include "../n110_square__/n110_200_rank.hpp"


class ConvRank {
public:

	static inline bool CONTAINS_OF10(const Rank r) { return (0 <= r) && (r < RankNum); }

	// 上下変換
	static inline constexpr Rank INVERSE10(const Rank r) { return RankNum - 1 - r; }

	// todo: アルファベットが辞書順に並んでいない処理系があるなら対応すること。
	static inline char TO_CHAR_USI10(const Rank r) {
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

	static inline char TO_CHAR_CSA10(const Rank r) { return '1' + r; }

	static inline Rank FROM_CHAR_CSA10(const char c) { return static_cast<Rank>(c - '1'); }

	static inline Rank FROM_CHAR_USI10(const char c) { return static_cast<Rank>(c - 'a'); }

};