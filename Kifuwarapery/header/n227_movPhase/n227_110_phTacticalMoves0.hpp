﻿#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


class MovePicker;


class PhTacticalMoves0 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		MoveStack* pMoveStack = UtilMoveStack::PickBest(movePicker.GetCurrMove(), movePicker.GetLastMove());
		movePicker.IncrementCurMove();

		if (pMoveStack->m_move != movePicker.GetTranspositionTableMove()) {
			assert(movePicker.GetCaptureThreshold() <= 0);

			if (movePicker.GetCaptureThreshold() <= movePicker.GetPos().GetSee(pMoveStack->m_move)) {
				resultMove = pMoveStack->m_move;
				return true;
			}

			// 後ろから SEE の点数が高い順に並ぶようにする。
			movePicker.GetEndBadCaptures()->m_move = pMoveStack->m_move;
			movePicker.DecrementEndBadCaptures();
		}

		return false;
	};

};


extern PhTacticalMoves0 g_phTacticalMoves0;
