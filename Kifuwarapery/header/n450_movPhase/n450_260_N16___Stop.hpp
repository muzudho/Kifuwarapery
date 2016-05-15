#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class N16___Stop : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 山札の底（最初）のカードに、カーソルを合わせます。
		nmEvent.BackToHome_CurrCard();

		// カードは作成せず、次のカードを最後のカードとして覚えておきます。
		nmEvent.SetSeekbarTerminated(nmEvent.GetCurrCard() + 1);

	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅ）
		//────────────────────────────────────────
		pickedCard = g_MOVE_NONE;
		return true;
	};

};


extern N16___Stop g_phStop;
