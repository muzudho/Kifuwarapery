#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// 山札の後ろからカードを積んでいき、カーソルは前へ戻す☆
// 山札の後ろには、 N01_PhTacticalMoves0 でカードを入れていた☆
class N05___BadCaptures : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 天札の頂点（最初）を、ファーストにします。
		nmEvent.GoToEndTalonCapacity_CurCard();

		// 天札の底（最後）を、ラストにします。
		nmEvent.SetSeekbarTerminated(nmEvent.GetBottom_SkyTalon());
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {
		
		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅの場合もあり☆？）
		//────────────────────────────────────────

		// 現在のムーブで確定☆
		pickedCard = nmEvent.GetCurrCard()->m_move;

		//────────────────────
		// カーソルは１つ前へ戻す☆
		//────────────────────
		nmEvent.GoPreviousCurCard();

		return true;
	};

};


extern N05___BadCaptures g_phBadCaptures;
