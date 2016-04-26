#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class PhStop : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, NextmoveEvent& nmEvent) const override {
		resultMove = g_MOVE_NONE;
		return true;
	};

	void GoNext2Phase(NextmoveEvent& nmEvent) override {
		nmEvent.SetLastMove(nmEvent.GetCurrMove() + 1);
	}

};


extern PhStop g_phStop;
