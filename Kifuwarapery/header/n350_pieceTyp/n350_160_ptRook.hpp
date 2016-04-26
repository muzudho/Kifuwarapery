#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_500_convSquare.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_150_rookAttackBb.hpp"
#include "../n160_board___/n160_230_setMaskBb.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "n350_040_ptEvent.hpp"
#include "n350_045_pieceTypeSeeEvent.hpp"
#include "n350_070_ptAbstract.hpp"


class PtRook : public PtAbstract {
public:

	PieceType GetNumber() const override {
		return PieceType::N06_Rook;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const override {
		return g_rookAttackBb.GetControllBb(ptEvent.m_occupied, ptEvent.m_sq);
	}

	// pin ‚ÍÈ‚©‚È‚¢B
	void Generate2RecaptureMoves(//FORCE_INLINE
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
	) const override {
		moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(this->GetNumber(), from, to, pos);

		if (
			ConvSquare::CAN_PROMOTE10(us, ConvSquare::TO_RANK10(to))
			|
			ConvSquare::CAN_PROMOTE10(us, ConvSquare::TO_RANK10(from))
		){
			MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);//, N00_Capture, this->GetNumber()
		}

		moveStackList++;
	}

	PieceType AppendToNextAttackerAndTryPromote(
		Bitboard& occupied,
		Bitboard& attackers,
		PieceType nextPT,
		const PieceTypeSeeEvent ptsEvent
		) const override;

};
