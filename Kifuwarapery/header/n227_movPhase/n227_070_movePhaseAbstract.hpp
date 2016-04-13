#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n228_movStack/n228_500_movePicker.hpp"


class MovePicker;


class MovePhaseAbstract {
public:

	virtual bool GetNext2Move( Move& resultMove, MovePicker& movePicker) const = 0;

	virtual void GoNext2Phase(MovePicker& movePicker) = 0;

};
