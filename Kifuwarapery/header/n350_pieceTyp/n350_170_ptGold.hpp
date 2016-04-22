#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_120_bishopAttackBb.hpp"
#include "../n160_board___/n160_130_lanceAttackBb.hpp"
#include "../n160_board___/n160_140_goldAttackBb.hpp"
#include "../n160_board___/n160_150_rookAttackBb.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "n350_040_ptEvent.hpp"
#include "n350_070_ptAbstract.hpp"


class PtGold : public PtAbstract {
public:

	PieceType GetNumber() const {//inline
		return PieceType::N07_Gold;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const {
		return g_goldAttackBb.GetControllBb(ptEvent.m_c, ptEvent.m_sq);
	}

	// pin ‚ÍÈ‚©‚È‚¢B//FORCE_INLINE
	void Generate2RecaptureMoves(
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
	) const {
		moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(this->GetNumber(), from, to, pos);

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
			pos.GetBbOf20(N02_Lance, turn))
			| (g_lanceAttackBb.GetControllBb(occupied, turn, to) &
				pos.GetBbOf20(N02_Lance, UtilColor::OppositeColor(turn)))
			| (g_rookAttackBb.GetControllBb(occupied, to) & pos.GetBbOf20(N06_Rook, N14_Dragon))
			| (g_bishopAttackBb.BishopAttack(occupied, to) & pos.GetBbOf20(N05_Bishop, N13_Horse));
		return attackers;
	}

};
