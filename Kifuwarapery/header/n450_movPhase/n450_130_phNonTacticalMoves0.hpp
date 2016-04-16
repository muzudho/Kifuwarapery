#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n374_genMove_/n374_500_generateMoves.hpp"
#include "../n374_genMove_/n374_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_400_hasPositiveScore.hpp"
#include "../n440_movStack/n440_500_movePicker.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class MovePicker;


class PhNonTacticalMoves0 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		Move move = movePicker.GetCurrMove()->m_move;
		movePicker.IncrementCurMove();
		if (move != movePicker.GetTranspositionTableMove()
			&& move != movePicker.GetKillerMove(0).m_move
			&& move != movePicker.GetKillerMove(1).m_move
			)
		{
			resultMove = move;
			return true;
		}
		return false;
	};

	void GoNext2Phase(MovePicker& movePicker) {
		movePicker.SetLastMove(MoveGenerator200::GenerateMoves_2<NonCaptureMinusPro>(movePicker.GetCurrMove(), movePicker.GetPos()));

		movePicker.ScoreNonCapturesMinusPro<false>();
		movePicker.SetCurrMove(movePicker.GetLastMove());

		movePicker.SetLastMoveAndLastNonCaputre(MoveGenerator200::GenerateMoves_2<Drop>(movePicker.GetCurrMove(), movePicker.GetPos()));
		movePicker.ScoreNonCapturesMinusPro<true>();

		movePicker.SetCurrMove(movePicker.GetFirstMove());
		movePicker.SetLastMove(std::partition(movePicker.GetCurrMove(), movePicker.GetLastNonCapture(), HasPositiveScore()));

		// 要素数は10個くらいまでであることが多い。要素数が少ないので、insertionSort() を使用する。
		UtilMoveStack::InsertionSort<MoveStack*, true>(movePicker.GetCurrMove(), movePicker.GetLastMove());
	}

};


extern PhNonTacticalMoves0 g_phNonTacticalMoves0;
