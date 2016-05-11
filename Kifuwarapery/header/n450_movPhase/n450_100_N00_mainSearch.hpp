#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class N00_MainSearch : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {
		UNREACHABLE;
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		// トランスポジション・テーブルを読むだけ☆？
		nmEvent.GoNextCurCard();

		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅの場合もあり☆？）
		//────────────────────────────────────────
		pickedCard = nmEvent.GetTranspositionTableMove();
		return true;
	};

};


extern N00_MainSearch g_mainSearch;
