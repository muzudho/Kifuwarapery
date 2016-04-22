#pragma once


#include <string>
#include "../n080_common__/n080_100_common.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 非クラスメンバ 静的グローバル・オブジェクト
//────────────────────────────────────────────────────────────────────────────────
static const std::string g_CONV_PIECE_TYPE_STRING_TABLE[PieceType::N15_PieceTypeNum] = {
	"", "FU", "KY", "KE", "GI", "KA", "HI", "KI", "OU", "TO", "NY", "NK", "NG", "UM", "RY"
};
//────────────────────────────────────────────────────────────────────────────────



class ConvPieceType {
public:

	static inline std::string GET_STRING(const PieceType pt) { return g_CONV_PIECE_TYPE_STRING_TABLE[pt]; }

	// pc が遠隔駒であるかを調べるのに使います。
	static const u32 SLIDER_VAL = 0x60646064;

	// pc が遠隔駒であるか
	static inline bool IS_SLIDER10(const PieceType pt) { return (ConvPieceType::SLIDER_VAL & (1 << pt)) != 0; }
	
	// 移動元から駒打ちの駒の種類に変換（未使用のもよう☆）
	static inline PieceType FROM_SRC_TO_DROP10(const Square from) { return static_cast<PieceType>(from - SquareNum + 1); }

};

