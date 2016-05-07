#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n350_pieceTyp/n350_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"


class MovePhaseAbstract {
public:

	virtual bool GetNext2Move( Move& resultMove, NextmoveEvent& nmEvent) const = 0;

	virtual void GoNext2Phase(NextmoveEvent& nmEvent) = 0;

};
