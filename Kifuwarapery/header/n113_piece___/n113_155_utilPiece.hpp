#pragma once

#include "../n113_piece___/n113_150_piece.hpp"


class UtilPiece {
public:
	// GetP == Empty �̂Ƃ��APieceType �� OccuPied �ɂȂ��Ă��܂��̂ŁA
	// Position::bbOf(UtilPiece::pieceToPieceType(GetP)) �Ƃ���ƁA
	// Position::emptyBB() �ł͂Ȃ� Position::occupiedBB() �ɂȂ��Ă��܂��̂ŁA
	// ���ӂ��邱�ƁB�o����ΏC���������B
	static inline PieceType ToPieceType(const Piece p) { return static_cast<PieceType>(p & 15); }


	static inline Piece Inverse(const Piece pc) { return static_cast<Piece>(pc ^ 0x10); }


	static inline Color ToColor(const Piece p) {
		assert(p != Empty);
		return static_cast<Color>(p >> 4);
	}


	static inline Piece FromColorAndPieceType(const Color c, const PieceType pt) { return static_cast<Piece>((c << 4) | pt); }


	// pc �����u��ł��邩
	static inline bool IsSlider(const Piece     pc) { return (g_isSliderVal & (1 << pc)) != 0; }
};
