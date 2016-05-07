#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class MovePhaseNum : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, NextmoveEvent& nmEvent) const override {
		UNREACHABLE;
		return false;
	};

	void GoNext2Phase(NextmoveEvent& nmEvent) override {
		UNREACHABLE;
	}

};


extern MovePhaseNum g_movePhaseNum;
