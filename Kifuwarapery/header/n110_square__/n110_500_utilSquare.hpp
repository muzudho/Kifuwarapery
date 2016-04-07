#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_300_direction.hpp"
#include "../n160_board___/n160_250_squareRelation.hpp"




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

	// s が Square の中に含まれているか判定
	static inline bool ContainsOf(const Square s) { return (0 <= s) && (s < SquareNum); }
	// File, Rank のどちらかがおかしいかも知れない時は、
	// こちらを使う。
	// こちらの方が遅いが、どんな File, Rank にも対応している。
	static inline bool ContainsOf(const File f, const Rank r) { return UtilFile::ContainsOf(f) && UtilRank::ContainsOf(r); }

	// 速度が必要な場面で使用するなら、テーブル引きの方が有効だと思う。
	static inline constexpr Square FromFileRank(const File f, const Rank r) {
		return static_cast<Square>(static_cast<int>(f) * 9 + static_cast<int>(r));
	}

	// 速度が必要な場面で使用する。
	static inline Rank ToRank(const Square s) {
		assert(UtilSquare::ContainsOf(s));
		return SquareToRank[s];
	}
	static inline File ToFile(const Square s) {
		assert(UtilSquare::ContainsOf(s));
		return SquareToFile[s];
	}




	static inline std::string ToStringUSI(const Square sq) {
		const Rank r = UtilSquare::ToRank(sq);
		const File f = UtilSquare::ToFile(sq);
		const char ch[] = { UtilFile::ToCharUSI(f), UtilRank::ToCharUSI(r), '\0' };
		return std::string(ch);
	}



	static inline std::string ToStringCSA(const Square sq) {
		const Rank r = UtilSquare::ToRank(sq);
		const File f = UtilSquare::ToFile(sq);
		const char ch[] = { UtilFile::ToCharCSA(f), UtilRank::ToCharCSA(r), '\0' };
		return std::string(ch);
	}


	// 後手の位置を先手の位置へ変換
	static inline constexpr Square Inverse(const Square sq) { return SquareNum - 1 - sq; }
	// Square の左右だけ変換
	static inline Square InverseFile(const Square sq) { return UtilSquare::FromFileRank(UtilFile::Inverse(UtilSquare::ToFile(sq)), UtilSquare::ToRank(sq)); }

	static inline constexpr Square InverseIfWhite(const Color c, const Square sq) { return (c == Black ? sq : Inverse(sq)); }

	static inline bool CanPromote(const Color c, const Rank fromOrToRank) {
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



