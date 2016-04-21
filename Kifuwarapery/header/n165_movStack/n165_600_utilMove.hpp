#pragma once

#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_155_utilPiece.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"


class UtilMove {
public:

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
	static inline Move GetCapturedPieceType2Move1(const PieceType captured) { return static_cast<Move>(captured << 20); }

	// 移動元、移動先、移動する駒の種類から指し手に変換
	static inline Move MakeMove(const PieceType pt, const Square from, const Square to) {
		return UtilMove::GetPieceType2Move(pt) | UtilMove::From2Move(from) | UtilMove::To2Move(to);
	}

	// 駒打ちの makeMove()
	// todo: PieceType を HandPiece に変更
	static inline Move MakeDropMove(const PieceType pt, const Square to) {
		return UtilMove::From2Move(UtilMove::GetDrop2From(pt)) | UtilMove::To2Move(to);
	}

};
