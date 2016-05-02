#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n407_moveGen_/n407_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class PhQCaptures1 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, NextmoveEvent& nmEvent) const override {
		Move move = UtilMoveStack::PickBest(nmEvent.GetCurrMove(), nmEvent.GetLastMove())->m_move;
		nmEvent.IncrementCurMove();
		assert(move.To() == nmEvent.GetRecaptureSquare());
		resultMove = move;
		return true;
	};

	// virtual の派生クラスなので template化はできないぜ☆（＾ｑ＾）
	void GoNext2Phase(NextmoveEvent& nmEvent) override {
		const Color us = nmEvent.GetPos().GetTurn();

		nmEvent.SetLastMove(
			us == Color::Black
			?
			g_moveGenerator200.GenerateMoves_recapture<Color::Black, Color::White>(nmEvent.GetFirstMove(), nmEvent.GetPos(), nmEvent.GetRecaptureSquare())
			:
			g_moveGenerator200.GenerateMoves_recapture<Color::White, Color::Black>(nmEvent.GetFirstMove(), nmEvent.GetPos(), nmEvent.GetRecaptureSquare())
		);//<Recapture>
		nmEvent.ScoreCaptures();
	}

};


extern PhQCaptures1 g_phQCaptures1;
