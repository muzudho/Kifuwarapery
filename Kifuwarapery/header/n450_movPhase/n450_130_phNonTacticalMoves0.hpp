#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n350_pieceTyp/n350_020_moveStack.hpp"
#include "../n407_moveGen_/n407_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_400_hasPositiveScore.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class PhNonTacticalMoves0 : public MovePhaseAbstract {
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
		nmEvent.SetLastMove(g_moveGenerator200.GenerateMoves_2(N04_NonCaptureMinusPro,nmEvent.GetCurrMove(), nmEvent.GetPos()));

		nmEvent.ScoreNonCapturesMinusPro<false>();
		nmEvent.SetCurrMove(nmEvent.GetLastMove());

		nmEvent.SetLastMoveAndLastNonCaputre(g_moveGenerator200.GenerateMoves_2(N02_Drop,nmEvent.GetCurrMove(), nmEvent.GetPos()));
		nmEvent.ScoreNonCapturesMinusPro<true>();

		nmEvent.SetCurrMove(nmEvent.GetFirstMove());
		nmEvent.SetLastMove(std::partition(nmEvent.GetCurrMove(), nmEvent.GetLastNonCapture(), HasPositiveScore()));

		// 要素数は10個くらいまでであることが多い。要素数が少ないので、insertionSort() を使用する。
		UtilMove01::InsertionSort<MoveStack*, true>(nmEvent.GetCurrMove(), nmEvent.GetLastMove());
	}

};


extern PhNonTacticalMoves0 g_phNonTacticalMoves0;
