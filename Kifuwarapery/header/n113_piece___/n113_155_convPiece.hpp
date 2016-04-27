#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_105_convPieceType.hpp"	// g_isSliderVal
#include "../n113_piece___/n113_150_piece.hpp"


namespace {
	//────────────────────────────────────────────────────────────────────────────────
	// 非クラスメンバ 秘密 静的グローバル・オブジェクト
	//────────────────────────────────────────────────────────────────────────────────

	//────────────────────────────────────────────────────────────────────────────────
	// きふわらべは 計算が苦手なので、早見表を作っておこうぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	/*
	// GetP == Empty のとき、PieceType は OccuPied になってしまうので、
	// Position::bbOf(UtilPiece::pieceToPieceType(GetP)) とすると、
	// Position::emptyBB() ではなく Position::occupiedBB() になってしまうので、
	// 注意すること。出来れば修正したい。
	static inline constexpr PieceType G_TO_PIECE_TYPE10(const Piece p) { return static_cast<PieceType>(p & 15); }
	static const PieceType g_CONV_PIECE_TO_PIECE_TYPE10[g_PIECE_NUM] = {
		G_TO_PIECE_TYPE10(Piece::N00_Empty),
		G_TO_PIECE_TYPE10(Piece::N01_BPawn),
		G_TO_PIECE_TYPE10(Piece::N02_BLance),
		G_TO_PIECE_TYPE10(Piece::N03_BKnight),
		G_TO_PIECE_TYPE10(Piece::N04_BSilver),
		G_TO_PIECE_TYPE10(Piece::N05_BBishop),
		G_TO_PIECE_TYPE10(Piece::N06_BRook),
		G_TO_PIECE_TYPE10(Piece::N07_BGold),
		G_TO_PIECE_TYPE10(Piece::N08_BKing),
		G_TO_PIECE_TYPE10(Piece::N09_BProPawn),
		G_TO_PIECE_TYPE10(Piece::N10_BProLance),
		G_TO_PIECE_TYPE10(Piece::N11_BProKnight),
		G_TO_PIECE_TYPE10(Piece::N12_BProSilver),
		G_TO_PIECE_TYPE10(Piece::N13_BHorse),
		G_TO_PIECE_TYPE10(Piece::N14_BDragon),
		G_TO_PIECE_TYPE10(Piece::N00_Empty),//15は欠番
		G_TO_PIECE_TYPE10(Piece::N00_Empty),//16は欠番
		G_TO_PIECE_TYPE10(Piece::N17_WPawn),
		G_TO_PIECE_TYPE10(Piece::N18_WLance),
		G_TO_PIECE_TYPE10(Piece::N19_WKnight),
		G_TO_PIECE_TYPE10(Piece::N20_WSilver),
		G_TO_PIECE_TYPE10(Piece::N21_WBishop),
		G_TO_PIECE_TYPE10(Piece::N22_WRook),
		G_TO_PIECE_TYPE10(Piece::N23_WGold),
		G_TO_PIECE_TYPE10(Piece::N24_WKing),
		G_TO_PIECE_TYPE10(Piece::N25_WProPawn),
		G_TO_PIECE_TYPE10(Piece::N26_WProLance),
		G_TO_PIECE_TYPE10(Piece::N27_WProKnight),
		G_TO_PIECE_TYPE10(Piece::N28_WProSilver),
		G_TO_PIECE_TYPE10(Piece::N29_WHorse),
		G_TO_PIECE_TYPE10(Piece::N30_WDragon),
		G_TO_PIECE_TYPE10(Piece::N31_PieceNone)
	};
	*/

	//────────────────────────────────────────────────────────────────────────────────
	// bit演算１回ケチるなんて　どうかしているぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	static inline constexpr Piece G_INVERSE10(const Piece pc) { return static_cast<Piece>(pc ^ 0x10); }
	static const Piece g_CONV_PIECE_INVERSE10[g_PIECE_NUM] = {
		G_INVERSE10(Piece::N00_Empty),
		G_INVERSE10(Piece::N01_BPawn),
		G_INVERSE10(Piece::N02_BLance),
		G_INVERSE10(Piece::N03_BKnight),
		G_INVERSE10(Piece::N04_BSilver),
		G_INVERSE10(Piece::N05_BBishop),
		G_INVERSE10(Piece::N06_BRook),
		G_INVERSE10(Piece::N07_BGold),
		G_INVERSE10(Piece::N08_BKing),
		G_INVERSE10(Piece::N09_BProPawn),
		G_INVERSE10(Piece::N10_BProLance),
		G_INVERSE10(Piece::N11_BProKnight),
		G_INVERSE10(Piece::N12_BProSilver),
		G_INVERSE10(Piece::N13_BHorse),
		G_INVERSE10(Piece::N14_BDragon),
		G_INVERSE10(Piece::N00_Empty),//15は欠番
		G_INVERSE10(Piece::N00_Empty),//16は欠番
		G_INVERSE10(Piece::N17_WPawn),
		G_INVERSE10(Piece::N18_WLance),
		G_INVERSE10(Piece::N19_WKnight),
		G_INVERSE10(Piece::N20_WSilver),
		G_INVERSE10(Piece::N21_WBishop),
		G_INVERSE10(Piece::N22_WRook),
		G_INVERSE10(Piece::N23_WGold),
		G_INVERSE10(Piece::N24_WKing),
		G_INVERSE10(Piece::N25_WProPawn),
		G_INVERSE10(Piece::N26_WProLance),
		G_INVERSE10(Piece::N27_WProKnight),
		G_INVERSE10(Piece::N28_WProSilver),
		G_INVERSE10(Piece::N29_WHorse),
		G_INVERSE10(Piece::N30_WDragon),
		G_INVERSE10(Piece::N31_PieceNone)
	};

	/*
	//────────────────────────────────────────────────────────────────────────────────
	// bit演算１回ケチるなんて　どうかしているぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	static inline constexpr Color G_TO_COLOR10(const Piece p) {	return static_cast<Color>(p >> 4); }
	static const Color g_CONV_PIECE_TO_COLOR10[g_PIECE_NUM] = {
		G_TO_COLOR10(Piece::N00_Empty),
		G_TO_COLOR10(Piece::N01_BPawn),
		G_TO_COLOR10(Piece::N02_BLance),
		G_TO_COLOR10(Piece::N03_BKnight),
		G_TO_COLOR10(Piece::N04_BSilver),
		G_TO_COLOR10(Piece::N05_BBishop),
		G_TO_COLOR10(Piece::N06_BRook),
		G_TO_COLOR10(Piece::N07_BGold),
		G_TO_COLOR10(Piece::N08_BKing),
		G_TO_COLOR10(Piece::N09_BProPawn),
		G_TO_COLOR10(Piece::N10_BProLance),
		G_TO_COLOR10(Piece::N11_BProKnight),
		G_TO_COLOR10(Piece::N12_BProSilver),
		G_TO_COLOR10(Piece::N13_BHorse),
		G_TO_COLOR10(Piece::N14_BDragon),
		G_TO_COLOR10(Piece::N00_Empty),//15は欠番
		G_TO_COLOR10(Piece::N00_Empty),//16は欠番
		G_TO_COLOR10(Piece::N17_WPawn),
		G_TO_COLOR10(Piece::N18_WLance),
		G_TO_COLOR10(Piece::N19_WKnight),
		G_TO_COLOR10(Piece::N20_WSilver),
		G_TO_COLOR10(Piece::N21_WBishop),
		G_TO_COLOR10(Piece::N22_WRook),
		G_TO_COLOR10(Piece::N23_WGold),
		G_TO_COLOR10(Piece::N24_WKing),
		G_TO_COLOR10(Piece::N25_WProPawn),
		G_TO_COLOR10(Piece::N26_WProLance),
		G_TO_COLOR10(Piece::N27_WProKnight),
		G_TO_COLOR10(Piece::N28_WProSilver),
		G_TO_COLOR10(Piece::N29_WHorse),
		G_TO_COLOR10(Piece::N30_WDragon),
		G_TO_COLOR10(Piece::N31_PieceNone)
	};
	*/

	//────────────────────────────────────────────────────────────────────────────────
	// 暗算表を作るなら、複数回の演算を１回にしたいんだぜ☆ｗｗｗ（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// ピース が遠隔駒であるか
	static inline constexpr bool G_IS_SLIDER10(const Piece pc) { return (g_CONV_PIECE_TYPE_SLIDER_VAL & (1 << pc)) != 0; }
	static const bool g_CONV_PIECE_IS_SLIDER10[g_PIECE_NUM] = {
		G_IS_SLIDER10(Piece::N00_Empty),
		G_IS_SLIDER10(Piece::N01_BPawn),
		G_IS_SLIDER10(Piece::N02_BLance),
		G_IS_SLIDER10(Piece::N03_BKnight),
		G_IS_SLIDER10(Piece::N04_BSilver),
		G_IS_SLIDER10(Piece::N05_BBishop),
		G_IS_SLIDER10(Piece::N06_BRook),
		G_IS_SLIDER10(Piece::N07_BGold),
		G_IS_SLIDER10(Piece::N08_BKing),
		G_IS_SLIDER10(Piece::N09_BProPawn),
		G_IS_SLIDER10(Piece::N10_BProLance),
		G_IS_SLIDER10(Piece::N11_BProKnight),
		G_IS_SLIDER10(Piece::N12_BProSilver),
		G_IS_SLIDER10(Piece::N13_BHorse),
		G_IS_SLIDER10(Piece::N14_BDragon),
		G_IS_SLIDER10(Piece::N00_Empty),//15は欠番
		G_IS_SLIDER10(Piece::N00_Empty),//16は欠番
		G_IS_SLIDER10(Piece::N17_WPawn),
		G_IS_SLIDER10(Piece::N18_WLance),
		G_IS_SLIDER10(Piece::N19_WKnight),
		G_IS_SLIDER10(Piece::N20_WSilver),
		G_IS_SLIDER10(Piece::N21_WBishop),
		G_IS_SLIDER10(Piece::N22_WRook),
		G_IS_SLIDER10(Piece::N23_WGold),
		G_IS_SLIDER10(Piece::N24_WKing),
		G_IS_SLIDER10(Piece::N25_WProPawn),
		G_IS_SLIDER10(Piece::N26_WProLance),
		G_IS_SLIDER10(Piece::N27_WProKnight),
		G_IS_SLIDER10(Piece::N28_WProSilver),
		G_IS_SLIDER10(Piece::N29_WHorse),
		G_IS_SLIDER10(Piece::N30_WDragon),
		G_IS_SLIDER10(Piece::N31_PieceNone)
	};
}


class ConvPiece {
public:

	// ピースを、ピースタイプに変換だぜ☆（＾ｑ＾）
	static inline PieceType TO_PIECE_TYPE10(const Piece p) {
		return static_cast<PieceType>(p & 15);//ビット演算を使うぜ☆（＾ｑ＾）
		//return g_CONV_PIECE_TO_PIECE_TYPE10[p];//暗算表を使うぜ☆（＾ｑ＾）
	}

	// ピースの白黒を反転だぜ☆（＾ｑ＾）
	static inline Piece INVERSE10(const Piece pc) { return g_CONV_PIECE_INVERSE10[pc]; }//暗算表を使うぜ☆（＾ｑ＾）

	// ピースの白黒を取得だぜ☆（＾ｑ＾）
	static inline Color TO_COLOR10(const Piece p) {
		assert(p != N00_Empty);
		return static_cast<Color>(p >> 4);//ビット演算を使うぜ☆（＾ｑ＾）
		//return g_CONV_PIECE_TO_COLOR10[p];//暗算表を使うぜ☆（＾ｑ＾）
	}

	// ピース が遠隔駒であるかどうか調べるぜ☆（＾ｑ＾）
	static inline bool IS_SLIDER10(const Piece pc) { return g_CONV_PIECE_IS_SLIDER10[pc]; }//暗算表を使うぜ☆（＾ｑ＾）

	// これは計算しないとタイヘンか☆（＾ｑ＾）
	static inline Piece FROM_COLOR_AND_PIECE_TYPE10(const Color c, const PieceType pt) { return static_cast<Piece>((c << 4) | pt); }

};
