#pragma once

#include "n223_100_move.hpp"


// 成り flag
inline Move promoteFlag() { return static_cast<Move>(Move::m_PromoteFlag); }
inline Move moveNone() { return static_cast<Move>(Move::m_MoveNone); }

// 移動先から指し手に変換
inline Move to2Move(const Square to) { return static_cast<Move>(to << 0); }
// 移動元から指し手に変換
inline Move from2Move(const Square from) { return static_cast<Move>(from << 7); }

// 駒打ちの駒の種類から移動元に変換
// todo: PieceType を HandPiece に変更
inline Square drop2From(const PieceType pt) { return static_cast<Square>(SquareNum - 1 + pt); }

// 駒打ち(移動元)から指し手に変換
inline Move drop2Move(const PieceType pt) { return static_cast<Move>(drop2From(pt) << 7); }

// 移動する駒の種類から指し手に変換
inline Move pieceType2Move(const PieceType pt) { return static_cast<Move>(pt << 16); }

// 移動元から駒打ちの駒の種類に変換
inline PieceType from2Drop(const Square from) { return static_cast<PieceType>(from - SquareNum + 1); }

// 取った駒の種類から指し手に変換
inline Move capturedPieceType2Move(const PieceType captured) { return static_cast<Move>(captured << 20); }
// 移動先と、Position から 取った駒の種類を判別し、指し手に変換
// 駒を取らないときは、0 (MoveNone) を返す。
inline Move capturedPieceType2Move(const Square to, const Position& pos) {
	const PieceType captured = UtilPiece::ToPieceType( pos.GetPiece(to) );
	return capturedPieceType2Move(captured);
}

// 移動元、移動先、移動する駒の種類から指し手に変換
inline Move makeMove(const PieceType pt, const Square from, const Square to) {
	return pieceType2Move(pt) | from2Move(from) | to2Move(to);
}

// 取った駒を判別する必要がある。
// この関数は駒を取らないときにも使える。
inline Move makeCaptureMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
	return capturedPieceType2Move(to, pos) | makeMove(pt, from, to);
}

// makeCaptureMove() かつ 成り
inline Move makeCapturePromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
	return makeCaptureMove(pt, from, to, pos) | promoteFlag();
}

// 駒打ちの makeMove()
// todo: PieceType を HandPiece に変更
inline Move makeDropMove(const PieceType pt, const Square to) { return from2Move(drop2From(pt)) | to2Move(to); }

