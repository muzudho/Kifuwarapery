#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "n350_040_ptEvent.hpp"


class PtAbstract {
public:

	virtual PieceType GetNumber() const = 0;

	virtual Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const = 0;
	/*
	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const {
		return this->GetAttacks2From(ptEvent.m_occupied, ptEvent.m_c, ptEvent.m_sq);
	}
	*/

	//virtual Bitboard GetAttacks2From(const Bitboard& occupied, const Color c, const Square sq) const = 0;
	Bitboard GetAttacks2From(const Bitboard& occupied, const Color c, const Square sq) const {
		const PieceTypeEvent ptEvent(occupied, c, sq);
		return this->GetAttacks2From(ptEvent);
	}

	// pin は省かない。//FORCE_INLINE 
	virtual void Generate2RecaptureMoves(
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
		) const = 0;

};
