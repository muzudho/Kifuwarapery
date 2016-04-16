#pragma once


#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"


class Position;


struct CheckInfo {
	explicit CheckInfo(const Position&);
	Bitboard m_dcBB; // discoverd check candidates bitboard
	Bitboard m_pinned;
	Bitboard m_checkBB[N15_PieceTypeNum];
};
