#pragma once

#include "n223_100_move.hpp"

class UtilMove {
public:
	// 成り flag
	static inline Move GetPromoteFlag() { return static_cast<Move>(Move::m_PromoteFlag); }
	static inline Move GetMoveNone() { return static_cast<Move>(Move::m_MoveNone); }

	// 移動先から指し手に変換
	static inline Move To2Move(const Square to) { return static_cast<Move>(to << 0); }
	// 移動元から指し手に変換
	static inline Move From2Move(const Square from) { return static_cast<Move>(from << 7); }

	// 駒打ちの駒の種類から移動元に変換
	// todo: PieceType を HandPiece に変更
	static inline Square GetDrop2From(const PieceType pt) { return static_cast<Square>(SquareNum - 1 + pt); }

	// 駒打ち(移動元)から指し手に変換
	static inline Move GetDrop2Move(const PieceType pt) { return static_cast<Move>(UtilMove::GetDrop2From(pt) << 7); }

	// 移動する駒の種類から指し手に変換
	static inline Move GetPieceType2Move(const PieceType pt) { return static_cast<Move>(pt << 16); }

	// 移動元から駒打ちの駒の種類に変換
	static inline PieceType From2Drop(const Square from) { return static_cast<PieceType>(from - SquareNum + 1); }

	// 取った駒の種類から指し手に変換
	static inline Move GetCapturedPieceType2Move(const PieceType captured) { return static_cast<Move>(captured << 20); }
	// 移動先と、Position から 取った駒の種類を判別し、指し手に変換
	// 駒を取らないときは、0 (MoveNone) を返す。
	static inline Move GetCapturedPieceType2Move(const Square to, const Position& pos) {
		const PieceType captured = UtilPiece::ToPieceType(pos.GetPiece(to));
		return GetCapturedPieceType2Move(captured);
	}

	// 移動元、移動先、移動する駒の種類から指し手に変換
	static inline Move MakeMove(const PieceType pt, const Square from, const Square to) {
		return UtilMove::GetPieceType2Move(pt) | UtilMove::From2Move(from) | UtilMove::To2Move(to);
	}

	// 取った駒を判別する必要がある。
	// この関数は駒を取らないときにも使える。
	static inline Move MakeCaptureMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
		return UtilMove::GetCapturedPieceType2Move(to, pos) | UtilMove::MakeMove(pt, from, to);
	}

	// makeCaptureMove() かつ 成り
	static inline Move MakeCapturePromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
		return UtilMove::MakeCaptureMove(pt, from, to, pos) | UtilMove::GetPromoteFlag();
	}

	// 駒打ちの makeMove()
	// todo: PieceType を HandPiece に変更
	static inline Move MakeDropMove(const PieceType pt, const Square to) { return UtilMove::From2Move(UtilMove::GetDrop2From(pt)) | UtilMove::To2Move(to); }


};
