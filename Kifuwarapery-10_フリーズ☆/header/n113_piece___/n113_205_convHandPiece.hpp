#pragma once

#include <string>
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_200_handPiece.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 非クラスメンバ 静的グローバル・オブジェクト
//────────────────────────────────────────────────────────────────────────────────
// USI用持ち駒。２文字。
static const std::string g_CONV_HAND_PIECE_2LEN_STRING_TABLE[HandPiece::HandPieceNum] = {
	"P*", "L*", "N*", "S*", "G*", "B*", "R*"
};
static const char g_CONV_HAND_PIECE_1CHAR_TABLE[HandPiece::HandPieceNum] = {
	'P', 'L', 'N', 'S', 'G', 'B', 'R'
};


class ConvHandPiece {
public:
	static inline std::string GET_2CHARS_STRING(const HandPiece hp) {
		return g_CONV_HAND_PIECE_2LEN_STRING_TABLE[hp];
	}

	static inline HandPiece FromPieceType(const PieceType pt) { return g_pieceTypeToHandPieceTable[pt]; }
};


