#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_140_goldAttackBb.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "n350_040_ptEvent.hpp"
#include "n350_070_ptAbstract.hpp"


class PtProKnight : public PtAbstract {
public:

	PieceType GetNumber() const {
		return PieceType::N11_ProKnight;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const {
		return g_goldAttackBb.GetControllBb(ptEvent.m_c, ptEvent.m_sq);
	}
	/*
	Bitboard GetAttacks2From(const Bitboard& occupied, const Color c, const Square sq) const {
		const PieceTypeEvent ptEvent(occupied, c, sq);
		return this->GetAttacks2From(ptEvent);
	}
	*/

	// pin �͏Ȃ��Ȃ��B
	void Generate2RecaptureMoves(//FORCE_INLINE
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
		) const {
		moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(this->GetNumber(), from, to, pos);
		moveStackList++;
	}

};

