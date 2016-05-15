#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// キラー・ムーブ[0] でも [1] でもなければ☆
class N04___NonTacticalMoves1 : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		
		nmEvent.GoToTopTalon_CurrCard();// 山札の頂点（最後）のカードに、カーソルを合わせます。
		nmEvent.SetSeekbarTerminated(nmEvent.Get_LastNonCapture());

		// 現在地と、山の頂上は　同じ位置になっているんじゃないのかなんだぜ☆？（＾ｑ＾）？
		if (static_cast<Depth>(3 * OnePly) <= nmEvent.GetDepth()) {
			std::sort(nmEvent.GetCurrCard(), nmEvent.GetSeekbarTerminated(), std::greater<MoveStack>());
		}
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		Move currCard = nmEvent.GetCurrCard()->m_move;

		//────────────────────
		// 進める☆
		//────────────────────
		nmEvent.GoNextCurCard();

		if (
			// トランスポジション・テーブル・ムーブではなく、
			currCard != nmEvent.GetTranspositionTableMove()
			&&
			// キラームーブの［１］でも［２］でもないなら☆
			currCard != nmEvent.GetKillerMove(0).m_move
			&&
			currCard != nmEvent.GetKillerMove(1).m_move
			)
		{

			//────────────────────────────────────────
			// 確定
			//────────────────────────────────────────
			pickedCard = currCard;
			return true;
		}

		//────────────────────────────────────────
		// 未確定
		//────────────────────────────────────────
		return false;
	};

};


extern N04___NonTacticalMoves1 g_phNonTacticalMoves1;
