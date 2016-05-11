#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// 山札の後ろからカードを積んでいき、カーソルは前へ戻す☆
class N05_PhBadCaptures : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 山札の限界ＭＡＸ頂点（空間の最後）にカーソルを合わせる☆？？
		nmEvent.SetCurrCard(nmEvent.GetTalonZeroCard() + Move::m_MAX_LEGAL_MOVES - 1);

		nmEvent.SetTalonLastCard(nmEvent.GetEndBadCaptures());

	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {
		
		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅの場合もあり☆？）
		//────────────────────────────────────────

		// 現在のムーブで確定☆
		pickedCard = nmEvent.GetCurrCard()->m_move;

		// カーソルは１つ前へ戻す☆
		nmEvent.GoPreviousCurCard();

		return true;
	};

};


extern N05_PhBadCaptures g_phBadCaptures;
