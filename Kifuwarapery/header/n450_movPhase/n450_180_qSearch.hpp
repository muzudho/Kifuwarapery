#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n440_movStack/n440_500_movePicker.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class MovePicker;


class QSearch : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		movePicker.IncrementCurMove();
		resultMove = movePicker.GetTranspositionTableMove();
		return true;
	};

	void GoNext2Phase(MovePicker& movePicker) {
		// これが無いと、MainSearch の後に EvasionSearch が始まったりしてしまう。
		movePicker.SetPhase(GenerateMovePhase::N16_PH_Stop);
	}

};


extern QSearch g_qSearch;
