#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_105_convPieceType.hpp"	// g_isSliderVal
#include "../n113_piece___/n113_150_piece.hpp"


class ConvPiece {
public:
	// GetP == Empty のとき、PieceType は OccuPied になってしまうので、
	// Position::bbOf(UtilPiece::pieceToPieceType(GetP)) とすると、
	// Position::emptyBB() ではなく Position::occupiedBB() になってしまうので、
	// 注意すること。出来れば修正したい。
	static inline PieceType TO_PIECE_TYPE10(const Piece p) { return static_cast<PieceType>(p & 15); }


	static inline Piece INVERSE10(const Piece pc) { return static_cast<Piece>(pc ^ 0x10); }


	static inline Color TO_COLOR10(const Piece p) {
		assert(p != N00_Empty);
		return static_cast<Color>(p >> 4);
	}


	static inline Piece FROM_COLOR_AND_PIECE_TYPE10(const Color c, const PieceType pt) { return static_cast<Piece>((c << 4) | pt); }


	// pc が遠隔駒であるか
	static inline bool IS_SLIDER10(const Piece     pc) { return (g_CONV_PIECE_TYPE_SLIDER_VAL & (1 << pc)) != 0; }
};
