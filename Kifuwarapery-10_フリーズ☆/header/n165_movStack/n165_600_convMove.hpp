#pragma once

#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_155_convPiece.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"


// Ａ　→　指し手　変換、または
// 指し手　→　Ａ　変換。
class ConvMove {
public:

	// 移動先から指し手に変換
	static inline Move FROM_DST10(const Square dst) { return static_cast<Move>(dst << 0); }

	// 移動元から指し手に変換
	static inline Move FROM_SRC10(const Square from) { return static_cast<Move>(from << 7); }

	// 駒打ちの駒の種類から移動元に変換
	// todo: PieceType を HandPiece に変更
	static inline Square Convert10_Drop2From(const PieceType pt) { return static_cast<Square>(SquareNum - 1 + pt); }

	// 移動する駒の種類から指し手に変換
	static inline Move FROM_PIECE_TYPE10(const PieceType pt) { return static_cast<Move>(pt << 16); }

	// 取った駒の種類から指し手に変換
	static inline Move FROM_CAPTURED_PIECE_TYPE10(const PieceType captured) { return static_cast<Move>(captured << 20); }

	// 駒打ち(移動元)から指し手に変換
	static inline Move FROM_DROP20(const PieceType pt) { return static_cast<Move>(ConvMove::Convert10_Drop2From(pt) << 7); }

	// 移動元、移動先、移動する駒の種類から指し手に変換
	static inline Move FROM_PT_SRC_DST30(const PieceType pt, const Square from, const Square to) {
		return ConvMove::FROM_PIECE_TYPE10(pt) | ConvMove::FROM_SRC10(from) | ConvMove::FROM_DST10(to);
	}

	// 駒打ちの makeMove()
	// todo: PieceType を HandPiece に変更
	static inline Move Convert30_MakeDropMove(const PieceType pt, const Square to) {
		return ConvMove::FROM_SRC10(ConvMove::Convert10_Drop2From(pt)) | ConvMove::FROM_DST10(to);
	}

};
