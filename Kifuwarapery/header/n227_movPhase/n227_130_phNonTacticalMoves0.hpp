#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


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

	}

};


extern PhNonTacticalMoves0 g_phNonTacticalMoves0;
