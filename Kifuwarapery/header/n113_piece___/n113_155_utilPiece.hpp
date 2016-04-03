#pragma once

#include "../n113_piece___/n113_150_piece.hpp"


class UtilPiece {
public:
	// GetP == Empty のとき、PieceType は OccuPied になってしまうので、
	// Position::bbOf(UtilPiece::pieceToPieceType(GetP)) とすると、
	// Position::emptyBB() ではなく Position::occupiedBB() になってしまうので、
	// 注意すること。出来れば修正したい。
	static inline PieceType pieceToPieceType(const Piece p) { return static_cast<PieceType>(p & 15); }


	static inline Piece inverse(const Piece pc) { return static_cast<Piece>(pc ^ 0x10); }


	static inline Color pieceToColor(const Piece p) {
		assert(p != Empty);
		return static_cast<Color>(p >> 4);
	}


	static inline Piece colorAndPieceTypeToPiece(const Color c, const PieceType pt) { return static_cast<Piece>((c << 4) | pt); }


	// pc が遠隔駒であるか
	static inline bool IsSlider(const Piece     pc) { return (g_isSliderVal & (1 << pc)) != 0; }
};
