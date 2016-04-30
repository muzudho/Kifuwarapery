#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_convColor.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_150_file.hpp"
#include "../n110_square__/n110_155_convFile.hpp"
#include "../n110_square__/n110_200_rank.hpp"
#include "../n110_square__/n110_205_convRank.hpp"
#include "../n110_square__/n110_250_squareDelta.hpp"
#include "../n110_square__/n110_300_direction.hpp"




class ConvSquare {
public:

	// 先手のときは BRANK, 後手のときは WRANK より target が前の段にあるなら true を返す。
	static inline bool IS_IN_FRONT_OF10(Color us, Rank bRank, Rank wRank, const Rank target) {
		return (us == Color::Black ? (target < bRank) : (wRank < target));
	}

	template<Color US>
	static inline bool IS_BEHIND10(Rank bRank, Rank wRank, const Rank target)
	{
		return (US == Black ? (bRank < target) : (target < wRank));
	}
	static inline bool IS_BEHIND10(Color us, Rank bRank, Rank wRank, const Rank target)
	{
		return (us == Black ? (bRank < target) : (target < wRank));
	}

	static inline bool IS_LEFT_OF10(Color us, File bFile, File wFile, const File target) {
		return (us == Black ? (bFile < target) : (target < wFile));
	}

	static inline bool IS_RIGHT_OF10(Color us, File bFile, File wFile, const File target) {
		return (us == Black ? (target < bFile) : (wFile < target));
	}

	// s が Square の中に含まれているか判定
	static inline bool CONTAINS_OF10(const Square s) {
		return (0 <= s) && (s < SquareNum);
	}

	// File, Rank のどちらかがおかしいかも知れない時は、
	// こちらを使う。
	// こちらの方が遅いが、どんな File, Rank にも対応している。
	static inline bool CONTAINS_OF20(const File f, const Rank r) {
		return ConvFile::CONTAINS_OF10(f) && ConvRank::CONTAINS_OF10(r);
	}


	// 速度が必要な場面で使用するなら、テーブル引きの方が有効だと思う。
	static inline constexpr Square FROM_FILE_RANK10(const File f, const Rank r) {
		return static_cast<Square>(static_cast<int>(f) * 9 + static_cast<int>(r));
	}

	// 速度が必要な場面で使用する。
	static inline Rank TO_RANK10(const Square s) {
		assert(ConvSquare::CONTAINS_OF10(s));
		// 計算せず、テーブル引き☆（＾ｑ＾）
		return g_squareToRank[s];
	}

	static inline File TO_FILE10(const Square s) {
		assert(ConvSquare::CONTAINS_OF10(s));
		return g_squareToFile[s];
	}

	// 末尾の改行付きで☆
	static inline std::string TO_2CHARS_N0_STRING_USI40(const Square sq) {
		const Rank r = ConvSquare::TO_RANK10(sq);
		const File f = ConvSquare::TO_FILE10(sq);
		const char ch[] = { ConvFile::TO_CHAR_USI10(f), ConvRank::TO_CHAR_USI10(r), '\0' };
		return std::string(ch);
	}

	static inline std::string TO_STRING_CSA40(const Square sq) {
		const Rank r = ConvSquare::TO_RANK10(sq);
		const File f = ConvSquare::TO_FILE10(sq);
		const char ch[] = { ConvFile::TO_CHAR_CSA10(f), ConvRank::TO_CHAR_CSA10(r), '\0' };
		return std::string(ch);
	}

	// 後手の位置を先手の位置へ変換
	static inline constexpr Square INVERSE10(const Square sq) {
		return SquareNum - 1 - sq;
	}

	// Square の左右だけ変換
	static inline Square INVERSE_FILE40(const Square sq) {
		return ConvSquare::FROM_FILE_RANK10(
			ConvFile::INVERSE10(
				ConvSquare::TO_FILE10(sq)
			),
			ConvSquare::TO_RANK10(sq)
		);
	}

	static inline constexpr Square INVERSE_IF_WHITE20(const Color c, const Square sq) {
		return (c == Black ? sq : INVERSE10(sq));
	}

	template<Color US>
	static inline bool CAN_PROMOTE10(const Rank fromOrToRank) {
#if 1
		static_assert(Black == 0, "");
		static_assert(Rank9 == 0, "");
		return static_cast<bool>(0x1c00007u & (1u << ((US << 4) + fromOrToRank)));
#else
		// 同じ意味。
		return (US == Black ? IsInFrontOf<Black, Rank6, Rank4>(fromOrToRank) : IsInFrontOf<White, Rank6, Rank4>(fromOrToRank));
#endif
	}
	static inline bool CAN_PROMOTE10(const Color c, const Rank fromOrToRank) {
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



