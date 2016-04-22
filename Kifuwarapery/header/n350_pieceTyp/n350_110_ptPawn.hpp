#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_500_convSquare.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_130_lanceAttackBb.hpp"
#include "../n160_board___/n160_190_pawnAttackBb.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "n350_040_ptEvent.hpp"
#include "n350_070_ptAbstract.hpp"


class PtPawn : public PtAbstract {
public:

	PieceType GetNumber() const {
		return PieceType::N01_Pawn;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const {
		return g_pawnAttackBb.GetControllBb(ptEvent.m_c, ptEvent.m_sq);
	}

	// pin は省かない。
	void Generate2RecaptureMoves(//FORCE_INLINE
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
	) const {

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

	Bitboard AppendToNextAttacker(
		Bitboard& attackers,
		const Position& pos,
		const Square to,
		Bitboard& occupied,
		const Color turn
	) const {
		attackers |= (g_lanceAttackBb.GetControllBb(occupied, UtilColor::OppositeColor(turn), to) &
			(pos.GetBbOf20(N06_Rook, N14_Dragon) |
				pos.GetBbOf20(N02_Lance, turn)));
		return attackers;
	}

	PieceType TryPromoteNextAttacker(
		const PieceType PT,
		const Square to,
		const Color turn,
		const Square from
	) const {
		// 歩、香、桂は　陣地に飛び込んだとき、成れる時には成る☆
		if (ConvSquare::CAN_PROMOTE10(turn, ConvSquare::TO_RANK10(to))) {
			return PT + PTPromote;
		}
		// それ以外の駒種類は、そのまま返す☆
		return PT;
	}

};
