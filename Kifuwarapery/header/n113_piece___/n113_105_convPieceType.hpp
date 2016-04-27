#pragma once


#include <string>
#include "../n080_common__/n080_100_common.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 非クラスメンバ 公開 静的グローバル・オブジェクト
//────────────────────────────────────────────────────────────────────────────────
// pc が遠隔駒であるかを調べるのに使います。
static const u32 g_CONV_PIECE_TYPE_SLIDER_VAL = 0x60646064;


namespace {
	//────────────────────────────────────────────────────────────────────────────────
	// 非クラスメンバ 秘密 静的グローバル・オブジェクト
	//────────────────────────────────────────────────────────────────────────────────
	static const std::string g_CONV_PIECE_TYPE_STRING_TABLE[g_PIECETYPE_NUM] = {
		"", "FU", "KY", "KE", "GI", "KA", "HI", "KI", "OU", "TO", "NY", "NK", "NG", "UM", "RY"
	};

	//────────────────────────────────────────────────────────────────────────────────
	// きふわらべは 計算が苦手なので、早見表を作っておこうぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// pc が遠隔駒であるか
	static inline constexpr bool G_IS_SLIDER10(const PieceType pt) { return (g_CONV_PIECE_TYPE_SLIDER_VAL & (1 << pt)) != 0; }
	static const bool g_CONV_PIECE_TYPE_IS_SLIDER[g_PIECETYPE_NUM] = {
		G_IS_SLIDER10(PieceType::N00_Occupied),
		G_IS_SLIDER10(PieceType::N01_Pawn),
		G_IS_SLIDER10(PieceType::N02_Lance),
		G_IS_SLIDER10(PieceType::N03_Knight),
		G_IS_SLIDER10(PieceType::N04_Silver),
		G_IS_SLIDER10(PieceType::N05_Bishop),
		G_IS_SLIDER10(PieceType::N06_Rook),
		G_IS_SLIDER10(PieceType::N07_Gold),
		G_IS_SLIDER10(PieceType::N08_King),
		G_IS_SLIDER10(PieceType::N09_ProPawn),
		G_IS_SLIDER10(PieceType::N10_ProLance),
		G_IS_SLIDER10(PieceType::N11_ProKnight),
		G_IS_SLIDER10(PieceType::N12_ProSilver),
		G_IS_SLIDER10(PieceType::N13_Horse),
		G_IS_SLIDER10(PieceType::N14_Dragon)
	};

	//────────────────────────────────────────────────────────────────────────────────
	// 九九の暗算の早見表を作るようなものだぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	static inline constexpr PieceType G_FROM_SRC_TO_DROP10(const Square from) { return static_cast<PieceType>(from - Square::SquareNum + 1); }
	static const PieceType g_CONV_PIECE_TYPE_FROM_SRC_TO_DROP10[Square::SquareNum] = {
		G_FROM_SRC_TO_DROP10(Square::I9),//I
		G_FROM_SRC_TO_DROP10(Square::I8),
		G_FROM_SRC_TO_DROP10(Square::I7),
		G_FROM_SRC_TO_DROP10(Square::I6),
		G_FROM_SRC_TO_DROP10(Square::I5),
		G_FROM_SRC_TO_DROP10(Square::I4),
		G_FROM_SRC_TO_DROP10(Square::I3),
		G_FROM_SRC_TO_DROP10(Square::I2),
		G_FROM_SRC_TO_DROP10(Square::I1),
		G_FROM_SRC_TO_DROP10(Square::H9),//H
		G_FROM_SRC_TO_DROP10(Square::H8),
		G_FROM_SRC_TO_DROP10(Square::H7),
		G_FROM_SRC_TO_DROP10(Square::H6),
		G_FROM_SRC_TO_DROP10(Square::H5),
		G_FROM_SRC_TO_DROP10(Square::H4),
		G_FROM_SRC_TO_DROP10(Square::H3),
		G_FROM_SRC_TO_DROP10(Square::H2),
		G_FROM_SRC_TO_DROP10(Square::H1),
		G_FROM_SRC_TO_DROP10(Square::G9),//G
		G_FROM_SRC_TO_DROP10(Square::G8),
		G_FROM_SRC_TO_DROP10(Square::G7),
		G_FROM_SRC_TO_DROP10(Square::G6),
		G_FROM_SRC_TO_DROP10(Square::G5),
		G_FROM_SRC_TO_DROP10(Square::G4),
		G_FROM_SRC_TO_DROP10(Square::G3),
		G_FROM_SRC_TO_DROP10(Square::G2),
		G_FROM_SRC_TO_DROP10(Square::G1),
		G_FROM_SRC_TO_DROP10(Square::F9),//F
		G_FROM_SRC_TO_DROP10(Square::F8),
		G_FROM_SRC_TO_DROP10(Square::F7),
		G_FROM_SRC_TO_DROP10(Square::F6),
		G_FROM_SRC_TO_DROP10(Square::F5),
		G_FROM_SRC_TO_DROP10(Square::F4),
		G_FROM_SRC_TO_DROP10(Square::F3),
		G_FROM_SRC_TO_DROP10(Square::F2),
		G_FROM_SRC_TO_DROP10(Square::F1),
		G_FROM_SRC_TO_DROP10(Square::E9),//E
		G_FROM_SRC_TO_DROP10(Square::E8),
		G_FROM_SRC_TO_DROP10(Square::E7),
		G_FROM_SRC_TO_DROP10(Square::E6),
		G_FROM_SRC_TO_DROP10(Square::E5),
		G_FROM_SRC_TO_DROP10(Square::E4),
		G_FROM_SRC_TO_DROP10(Square::E3),
		G_FROM_SRC_TO_DROP10(Square::E2),
		G_FROM_SRC_TO_DROP10(Square::E1),
		G_FROM_SRC_TO_DROP10(Square::D9),//D
		G_FROM_SRC_TO_DROP10(Square::D8),
		G_FROM_SRC_TO_DROP10(Square::D7),
		G_FROM_SRC_TO_DROP10(Square::D6),
		G_FROM_SRC_TO_DROP10(Square::D5),
		G_FROM_SRC_TO_DROP10(Square::D4),
		G_FROM_SRC_TO_DROP10(Square::D3),
		G_FROM_SRC_TO_DROP10(Square::D2),
		G_FROM_SRC_TO_DROP10(Square::D1),
		G_FROM_SRC_TO_DROP10(Square::C9),//C
		G_FROM_SRC_TO_DROP10(Square::C8),
		G_FROM_SRC_TO_DROP10(Square::C7),
		G_FROM_SRC_TO_DROP10(Square::C6),
		G_FROM_SRC_TO_DROP10(Square::C5),
		G_FROM_SRC_TO_DROP10(Square::C4),
		G_FROM_SRC_TO_DROP10(Square::C3),
		G_FROM_SRC_TO_DROP10(Square::C2),
		G_FROM_SRC_TO_DROP10(Square::C1),
		G_FROM_SRC_TO_DROP10(Square::B9),//B
		G_FROM_SRC_TO_DROP10(Square::B8),
		G_FROM_SRC_TO_DROP10(Square::B7),
		G_FROM_SRC_TO_DROP10(Square::B6),
		G_FROM_SRC_TO_DROP10(Square::B5),
		G_FROM_SRC_TO_DROP10(Square::B4),
		G_FROM_SRC_TO_DROP10(Square::B3),
		G_FROM_SRC_TO_DROP10(Square::B2),
		G_FROM_SRC_TO_DROP10(Square::B1),
		G_FROM_SRC_TO_DROP10(Square::A9),//A
		G_FROM_SRC_TO_DROP10(Square::A8),
		G_FROM_SRC_TO_DROP10(Square::A7),
		G_FROM_SRC_TO_DROP10(Square::A6),
		G_FROM_SRC_TO_DROP10(Square::A5),
		G_FROM_SRC_TO_DROP10(Square::A4),
		G_FROM_SRC_TO_DROP10(Square::A3),
		G_FROM_SRC_TO_DROP10(Square::A2),
		G_FROM_SRC_TO_DROP10(Square::A1)
	};

	//────────────────────────────────────────────────────────────────────────────────
}


// 暗算表（＾ｑ＾）
class ConvPieceType {
public:

	static inline std::string GET_STRING(const PieceType pt) {
		return g_CONV_PIECE_TYPE_STRING_TABLE[pt];
	}
	
	static inline bool IS_SLIDER10(const PieceType pt) {
		return g_CONV_PIECE_TYPE_IS_SLIDER[pt];
	}

	// 移動元から駒打ちの駒の種類に変換（未使用のもよう☆）
	static inline PieceType FROM_SRC_TO_DROP10(const Square from) {
		return g_CONV_PIECE_TYPE_FROM_SRC_TO_DROP10[from];
	}

};

