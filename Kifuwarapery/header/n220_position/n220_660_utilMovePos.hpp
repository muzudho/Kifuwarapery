#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_155_convPiece.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"


class UtilMovePos {
public:

	// 移動先と、Position から （取った駒の種類 CapturedPieceType を判別し）　指し手に変換
	// 駒を取らないときは、0 (MoveNone) を返す。
	static inline Move GetCapturedPieceType2Move2(const Square dst, const Position& pos) {

		const PieceType captured = ConvPiece::TO_PIECE_TYPE10(pos.GetPiece(dst));

		return ConvMove::FROM_CAPTURED_PIECE_TYPE10(captured);
	}

	// 取った駒を判別する必要がある。
	// この関数は駒を取らないときにも使える。
	static inline Move MakeCaptureMove( // 新型☆（＾ｑ＾）
		const Move pieceTypeAsMove,
		const Square from,
		const Square to,
		const Position& pos
		) {

		return UtilMovePos::GetCapturedPieceType2Move2(to, pos) |
			ConvMove::FROM_PT_SRC_DST20( pieceTypeAsMove, from,	to);
	}
	static inline Move MakeCaptureMove( // 旧型☆（＾ｑ＾）
		const PieceType pt, // ここをムーブにできないか☆（＾ｑ＾）
		const Square from,
		const Square to,
		const Position& pos
		) {

		return UtilMovePos::GetCapturedPieceType2Move2(to, pos) |
			ConvMove::FROM_PT_SRC_DST30(
				pt,// ピースタイプを、ムーブに変換する決まりきった処理☆
				from,
				to
			);
	}

	// makeCaptureMove() かつ 成り
	static inline Move MakeCapturePromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
		return UtilMovePos::MakeCaptureMove(pt, from, to, pos) | g_MOVE_PROMOTE_FLAG;//UtilMove::GetPromoteFlag()
	}

};