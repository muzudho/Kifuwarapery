#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_025_utilMoveStack.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// N11_PhQEvasions と同じ☆？
class N07___Evasions : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 山札の底（最初）のカードに、カーソルを合わせます。
		nmEvent.BackToHome_CurrCard();

		// Ｎ０６（王手回避の手等）のカードを作成し、その最後のカードを覚えておきます。
		MoveStack* pNewTalon = g_moveGenerator200.GenerateMoves_2(N06_Evasion, nmEvent.GetCurrCard(), nmEvent.GetPos());
		nmEvent.SetSeekbarTerminated(pNewTalon);

		if (nmEvent.GetCurrCard() + 1 < nmEvent.GetSeekbarTerminated()) {
			nmEvent.DoScoreing_Evasions();
		}
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		Move bestCard = UtilMoveStack::PickBest_AndSort(nmEvent.GetCurrCard(), nmEvent.GetSeekbarTerminated())->m_move;

		//────────────────────
		// 進める☆
		//────────────────────
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


extern N07___Evasions g_phEvasions;
