﻿#pragma once

#include "../n113_piece___/n113_100_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n220_position/n220_100_repetitionType.hpp"

class Position;

struct CheckInfo {
	explicit CheckInfo(const Position&);
	Bitboard m_dcBB; // discoverd check candidates bitboard
	Bitboard m_pinned;
	Bitboard m_checkBB[PieceTypeNum];
};
