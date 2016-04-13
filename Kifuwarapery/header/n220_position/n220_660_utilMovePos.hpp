#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_155_utilPiece.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_650_position.hpp"


class UtilMovePos {
public:

	// 移動先と、Position から 取った駒の種類を判別し、指し手に変換
	// 駒を取らないときは、0 (MoveNone) を返す。
	static inline Move GetCapturedPieceType2Move2(const Square to, const Position& pos) {

		const PieceType captured = UtilPiece::ToPieceType(pos.GetPiece(to));

		return UtilMove::GetCapturedPieceType2Move1(captured);
	}

	// 取った駒を判別する必要がある。
	// この関数は駒を取らないときにも使える。
	static inline Move MakeCaptureMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
		return UtilMovePos::GetCapturedPieceType2Move2(to, pos) | UtilMove::MakeMove(pt, from, to);
	}

	// makeCaptureMove() かつ 成り
	static inline Move MakeCapturePromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
		return UtilMovePos::MakeCaptureMove(pt, from, to, pos) | UtilMove::GetPromoteFlag();
	}

};