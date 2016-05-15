#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_155_convPiece.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_420_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"


class UtilMovePos {
public:

	// 取った駒を Move書式に変換☆
	// 駒を取らないときは、0 (MoveNone) を返す。
	static inline Move BUILD_CARD_CAPTURED_PIECE_FROM_SQ(
		const Position& pos,	// （５）
		const Square dst		// （１）
	) {

		// 盤面から計算で求めるぜ☆（＾ｑ＾）
		return ConvMove::FROM_CAPTURED_PIECE_TYPE10(
			ConvPiece::TO_PIECE_TYPE10(
				pos.GetPiece(dst)//取った駒☆
				)
			);
	}

	// 取った駒を判別する必要がある。
	// この関数は駒を取らないときにも使える。
	static inline Move BUILD_CARD_CAPTURE(
		const Position& pos,			// （５）
		const Move pieceTypeAsMove,		// （４）
		const Square from,				// （２）
		const Square to					// （１）
		) {

		return
			UtilMovePos::BUILD_CARD_CAPTURED_PIECE_FROM_SQ(pos, to)
			|
			ConvMove::FROM_PT_SRC_DST20( pieceTypeAsMove, from,	to)
			;
	}

	// makeCaptureMove() かつ 成り
	static inline Move BUILD_CARD_CAPTURE_PROMOTE(
		const Position& pos,			// （５）
		const Move pieceTypeAsMove,		// （４）
		const Square from,				// （２）
		const Square to					// （１）
	) {
		return
			UtilMovePos::BUILD_CARD_CAPTURE(pos, pieceTypeAsMove, from, to)
			|
			g_MOVE_PROMOTE_FLAG
			;
	}

};