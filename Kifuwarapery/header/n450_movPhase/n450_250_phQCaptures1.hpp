#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n407_moveGen_/n407_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_movePicker.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class MovePicker;


class PhQCaptures1 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		Move move = UtilMoveStack::PickBest(movePicker.GetCurrMove(), movePicker.GetLastMove())->m_move;
		movePicker.IncrementCurMove();
		assert(move.To() == movePicker.GetRecaptureSquare());
		resultMove = move;
		return true;
	};

	void GoNext2Phase(MovePicker& movePicker) {
		movePicker.SetLastMove(g_moveGenerator200.GenerateMoves_3(movePicker.GetFirstMove(), movePicker.GetPos(), movePicker.GetRecaptureSquare()));//<Recapture>
		movePicker.ScoreCaptures();
	}

};


extern PhQCaptures1 g_phQCaptures1;
