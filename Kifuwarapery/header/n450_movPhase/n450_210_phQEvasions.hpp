#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n360_genMove_/n360_500_generateMoves.hpp"
#include "../n440_movStack/n440_500_movePicker.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class MovePicker;


class PhQEvasions : public MovePhaseAbstract {
public:

	bool GetNext2Move(Move& resultMove, MovePicker& movePicker) const {
		Move move = UtilMoveStack::PickBest(movePicker.GetCurrMove(), movePicker.GetLastMove())->m_move;
		movePicker.IncrementCurMove();
		if (move != movePicker.GetTranspositionTableMove()) {
			resultMove = move;
			return true;
		}
		return false;
	};

	void GoNext2Phase(MovePicker& movePicker) {
		movePicker.SetLastMove(generateMoves<Evasion>(movePicker.GetCurrMove(), movePicker.GetPos()));
		if (movePicker.GetCurrMove() + 1 < movePicker.GetLastMove()) {
			movePicker.ScoreEvasions();
		}
	}

};


extern PhQEvasions g_phQEvasions;
