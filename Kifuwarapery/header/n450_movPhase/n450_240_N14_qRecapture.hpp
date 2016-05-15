#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


class N14_QRecapture : public MovePhaseAbstract {
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

		// N16_Stop にフェーズ遷移しているので、ここにはこないぜ☆（＾ｑ＾）

		UNREACHABLE;
		return false;
	};

};


extern N14_QRecapture g_qRecapture;
