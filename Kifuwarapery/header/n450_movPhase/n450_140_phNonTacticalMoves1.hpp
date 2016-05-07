#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n350_pieceTyp/n350_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class PhNonTacticalMoves1 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, NextmoveEvent& nmEvent) const override {
		Move move = nmEvent.GetCurrMove()->m_move;
		nmEvent.IncrementCurMove();
		if (move != nmEvent.GetTranspositionTableMove()
			&& move != nmEvent.GetKillerMove(0).m_move
			&& move != nmEvent.GetKillerMove(1).m_move
			)
		{
			resultMove = move;
			return true;
		}
		return false;
	};

	void GoNext2Phase(NextmoveEvent& nmEvent) override {
		nmEvent.SetCurrMove(nmEvent.GetLastMove());
		nmEvent.SetLastMove(nmEvent.GetLastNonCapture());

		if (static_cast<Depth>(3 * OnePly) <= nmEvent.GetDepth() ) {
			std::sort(nmEvent.GetCurrMove(), nmEvent.GetLastMove(), std::greater<MoveStack>());
		}
	}

};


extern PhNonTacticalMoves1 g_phNonTacticalMoves1;
