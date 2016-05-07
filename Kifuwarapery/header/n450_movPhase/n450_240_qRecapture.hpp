#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n350_pieceTyp/n350_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class QRecapture : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, NextmoveEvent& nmEvent) const override {
		UNREACHABLE;
		return false;
	};

	void GoNext2Phase(NextmoveEvent& nmEvent) override {
		// これが無いと、MainSearch の後に EvasionSearch が始まったりしてしまう。
		nmEvent.SetPhase(GenerateMovePhase::N16_PH_Stop);
		nmEvent.SetLastMove(nmEvent.GetCurrMove() + 1);
	}

};


extern QRecapture g_qRecapture;
