#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


class MovePicker;


class PhQEvasions : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		Move move = UtilMoveStack::PickBest(movePicker.GetCurrMove(), movePicker.GetLastMove())->m_move;
		movePicker.IncrementCurMove();
		if (move != movePicker.GetTranspositionTableMove()) {
			resultMove = move;
			return true;
		}
		return false;
	};

};


extern PhQEvasions g_phQEvasions;
