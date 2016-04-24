#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"


class PieceTypeEvent {
public:
	const Bitboard& m_occupied;
	const Color m_c;
	const Square m_sq;

public:
	PieceTypeEvent(
		const Bitboard& occupied,
		const Color c,
		const Square sq
	) :
		m_occupied(occupied),//(^q^)修正：タイプミス　→　m_occupied(m_occupied),
		m_c(c),
		m_sq(sq)
	{

	}
};