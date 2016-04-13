#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "n227_070_movePhaseAbstract.hpp"


class MovePicker;


class PhKillers : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		Move move = movePicker.GetCurrMove()->m_move;
		movePicker.IncrementCurMove();
		if (!move.IsNone()
			&& move != movePicker.GetTranspositionTableMove()
			&& movePicker.GetPos().MoveIsPseudoLegal(move, true)
			&& movePicker.GetPos().GetPiece(move.To()) == Empty)
		{
			resultMove = move;
			return true;
		}
		return false;
	};

};


extern PhKillers g_phKillers;
