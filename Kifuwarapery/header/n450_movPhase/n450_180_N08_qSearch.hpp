#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class N08_QSearch : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// これが無いと、MainSearch の後に EvasionSearch が始まったりしてしまう。
		nmEvent.SetPhase(GenerateMovePhase::N16s_P1__End_Ph_Stop);

		// カードは作成せず、次のカードを最後のカードとして覚えておきます。
		nmEvent.SetTalonLastCard(nmEvent.GetCurrCard() + 1);
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅの場合もあり☆？）
		//────────────────────────────────────────

		// 次のムーブへ☆
		nmEvent.GoNextCurCard();

		// トランスポジション・テーブル・ムーブ　で確定☆
		pickedCard = nmEvent.GetTranspositionTableMove();

		return true;
	};

};


extern N08_QSearch g_qSearch;
