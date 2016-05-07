#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_025_utilMoveStack.hpp"
#include "../n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class PhQEvasions : public MovePhaseAbstract {
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
		nmEvent.SetLastMove(g_moveGenerator200.GenerateMoves_2(N06_Evasion,nmEvent.GetCurrMove(), nmEvent.GetPos()));
		if (nmEvent.GetCurrMove() + 1 < nmEvent.GetLastMove()) {
			nmEvent.ScoreEvasions();
		}
	}

};


extern PhQEvasions g_phQEvasions;
