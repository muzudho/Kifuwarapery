#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_500_utilSquare.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_190_pawnAttackBb.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "n350_070_ptAbstract.hpp"


class PtPawn : public PtAbstract {
public:

	inline PieceType GetNumber() const {
		return PieceType::N01_Pawn;
	}

	inline Bitboard GetAttacks2From(const Bitboard& occupied, const Color c, const Square sq) const {
		return g_pawnAttackBb.GetControllBb(c, sq);;
	}

	// pin ‚ÍÈ‚©‚È‚¢B
	FORCE_INLINE void Generate2RecaptureMoves(
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
		) const {
		(*moveStackList++).m_move = (
			(UtilSquare::CanPromote(us, UtilSquare::ToRank(to)) | UtilSquare::CanPromote(us, UtilSquare::ToRank(from))
				) ?
			g_makePromoteMove.MakePromoteMove2(N00_Capture, this->GetNumber(), from, to, pos) :
			g_makePromoteMove.MakeNonPromoteMove(N00_Capture, this->GetNumber(), from, to, pos)
		);
	}

};
