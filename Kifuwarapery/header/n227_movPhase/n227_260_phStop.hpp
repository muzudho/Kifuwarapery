#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


class MovePicker;


class PhStop : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		resultMove = Move::GetMoveNone();
		return true;
	};

};


extern PhStop g_phStop;
