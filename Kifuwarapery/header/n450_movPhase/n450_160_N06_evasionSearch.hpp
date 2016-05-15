#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// QEvasionSearch と同じ内容☆？
class N06_EvasionSearch : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 山札の底（最初）のカードに、カーソルを合わせます。
		nmEvent.BackToHome_CurrCard();

		// これが無いと、MainSearch の後に EvasionSearch が始まったりしてしまう。
		nmEvent.SetPhase(GenerateMovePhase::N16_xxxxx_P1__Stop);

		// カードは作成せず、次のカードを最後のカードとして覚えておきます。
		nmEvent.SetSeekbarTerminated(nmEvent.GetCurrCard() + 1);
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		//────────────────────
		// 進める☆
		//────────────────────
		nmEvent.GoNextCurCard();

		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅの場合もあり☆？）
		//────────────────────────────────────────

		// トランスポジション・テーブル・ムーブ　で確定☆
		pickedCard = nmEvent.GetTranspositionTableMove();
		return true;
	};

};


extern N06_EvasionSearch g_evasionSearch;
