#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


class MovePicker;


class MainSearch : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		movePicker.IncrementCurMove();
		resultMove = movePicker.GetTranspositionTableMove();
		return true;
	};

	void GoNext2Phase(MovePicker& movePicker) {

	}

};


extern MainSearch g_mainSearch;
