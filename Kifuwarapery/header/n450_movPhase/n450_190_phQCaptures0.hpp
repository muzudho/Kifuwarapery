#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n350_pieceTyp/n350_020_moveStack.hpp"
#include "../n350_pieceTyp/n350_025_utilMoveStack.hpp"
#include "../n407_moveGen_/n407_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class PhQCaptures0 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, NextmoveEvent& nmEvent) const override {
		Move move = UtilMoveStack::PickBest(nmEvent.GetCurrMove(), nmEvent.GetLastMove())->m_move;
		nmEvent.IncrementCurMove();
		if (move != nmEvent.GetTranspositionTableMove()) {
			resultMove = move;
			return true;
		}
		return false;
	};

	void GoNext2Phase(NextmoveEvent& nmEvent) override {
		nmEvent.SetLastMove(g_moveGenerator200.GenerateMoves_2(N03_CapturePlusPro,nmEvent.GetFirstMove(), nmEvent.GetPos()));
		nmEvent.ScoreCaptures();
	}

};


extern PhQCaptures0 g_phQCaptures0;
