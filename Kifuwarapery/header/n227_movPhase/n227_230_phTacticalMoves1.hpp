﻿#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


class MovePicker;


class PhTacticalMoves1 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		MoveStack* pMoveStack = UtilMoveStack::PickBest(movePicker.GetCurrMove(), movePicker.GetLastMove());
		movePicker.IncrementCurMove();

		// todo: see が確実に駒打ちじゃないから、内部で駒打ちか判定してるのは少し無駄。
		if (pMoveStack->m_move != movePicker.GetTranspositionTableMove() && movePicker.GetCaptureThreshold() < movePicker.GetPos().GetSee(pMoveStack->m_move)) {
			resultMove = pMoveStack->m_move;
			return true;
		}
		return false;
	};

};


extern PhTacticalMoves1 g_phTacticalMoves1;
