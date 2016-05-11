﻿#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_025_utilMoveStack.hpp"
#include "../n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// N07_PhEvasions と同じ☆？
class N11_PhQEvasions : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		nmEvent.SetTalonLastCard(
			// Ｎ０６（王手回避の手等）のカードを作成し、その最後のカードを覚えておきます。
			g_moveGenerator200.GenerateMoves_2(N06_Evasion, nmEvent.GetCurrCard(), nmEvent.GetPos())
		);

		if (nmEvent.GetCurrCard() + 1 < nmEvent.GetTalonLastCard()) {
			nmEvent.ScoreEvasions();
		}
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		Move bestCard = UtilMoveStack::PickBest(nmEvent.GetCurrCard(), nmEvent.GetTalonLastCard())->m_move;

		nmEvent.GoNextCurCard();

		if (bestCard != nmEvent.GetTranspositionTableMove()) {

			//────────────────────────────────────────
			// 確定
			//────────────────────────────────────────
			pickedCard = bestCard;
			return true;
		}

		//────────────────────────────────────────
		// 未確定
		//────────────────────────────────────────
		return false;
	};

};


extern N11_PhQEvasions g_phQEvasions;
