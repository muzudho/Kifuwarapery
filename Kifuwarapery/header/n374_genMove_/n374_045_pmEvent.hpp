#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n374_genMove_/n374_040_mtEvent.hpp"


class PieceMovesEvent {
public:
	const MoveType m_mt;
	MoveTypeEvent& m_mtEvent;//変化するぜ☆
	const	bool m_all;
	const Bitboard& m_target;
	const Square m_ksq;

public:
	PieceMovesEvent(
		const MoveType mt,
		MoveTypeEvent& mtEvent,
		const	bool all,
		const Bitboard& target,
		const Square ksq
	) :
		m_mt(mt),
		m_mtEvent(mtEvent),
		m_all(all),
		m_target(target),
		m_ksq(ksq)
	{

	}
};