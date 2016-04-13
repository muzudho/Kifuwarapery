#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


class MovePicker;


class MovePhaseNum : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		UNREACHABLE;
		return false;
	};

};


extern MovePhaseNum g_movePhaseNum;
