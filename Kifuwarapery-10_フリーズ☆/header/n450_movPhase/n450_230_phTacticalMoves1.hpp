#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n407_moveGen_/n407_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class PhTacticalMoves1 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, NextmoveEvent& nmEvent) const {
		MoveStack* pMoveStack = UtilMoveStack::PickBest(nmEvent.GetCurrMove(), nmEvent.GetLastMove());
		nmEvent.IncrementCurMove();

		// todo: see が確実に駒打ちじゃないから、内部で駒打ちか判定してるのは少し無駄。
		if (pMoveStack->m_move != nmEvent.GetTranspositionTableMove() && nmEvent.GetCaptureThreshold() < nmEvent.GetPos().GetSee(pMoveStack->m_move)) {
			resultMove = pMoveStack->m_move;
			return true;
		}
		return false;
	};

	void GoNext2Phase(NextmoveEvent& nmEvent) {
		nmEvent.SetLastMove(g_moveGenerator200.GenerateMoves_2(N03_CapturePlusPro,nmEvent.GetCurrMove(), nmEvent.GetPos()));
		nmEvent.ScoreCaptures();
	}

};


extern PhTacticalMoves1 g_phTacticalMoves1;
