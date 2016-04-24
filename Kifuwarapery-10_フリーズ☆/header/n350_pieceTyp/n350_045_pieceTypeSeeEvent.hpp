#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n220_position/n220_650_position.hpp"


class PieceTypeSeeEvent {
public:
	const Position& m_pos;
	const Square m_to;
	const Bitboard& m_opponentAttackers;
	const Color m_turn;

public:
	PieceTypeSeeEvent(
		const Position& pos,
		const Square to,
		const Bitboard& opponentAttackers,
		const Color turn
	) :
		m_pos(pos),
		m_to(to),
		m_opponentAttackers(opponentAttackers),
		m_turn(turn)
	{

	}

};