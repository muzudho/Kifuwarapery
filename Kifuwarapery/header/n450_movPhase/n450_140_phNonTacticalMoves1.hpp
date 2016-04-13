#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n440_movStack/n440_500_movePicker.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class MovePicker;


class PhNonTacticalMoves1 : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		Move move = movePicker.GetCurrMove()->m_move;
		movePicker.IncrementCurMove();
		if (move != movePicker.GetTranspositionTableMove()
			&& move != movePicker.GetKillerMove(0).m_move
			&& move != movePicker.GetKillerMove(1).m_move
			)
		{
			resultMove = move;
			return true;
		}
		return false;
	};

	void GoNext2Phase(MovePicker& movePicker) {
		movePicker.SetCurrMove(movePicker.GetLastMove());
		movePicker.SetLastMove(movePicker.GetLastNonCapture());

		if (static_cast<Depth>(3 * OnePly) <= movePicker.GetDepth() ) {
			std::sort(movePicker.GetCurrMove(), movePicker.GetLastMove(), std::greater<MoveStack>());
		}
	}

};


extern PhNonTacticalMoves1 g_phNonTacticalMoves1;
