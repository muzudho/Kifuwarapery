#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// キラー・ムーブが２つあるので、その２枚を読もうぜ、という仕組みだぜ☆
// 山札ではないところを読むぜ☆
class N02___Killers : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// キラー・ムーブが２つあるので、その２枚を読もうぜ、という仕組みだぜ☆
		nmEvent.SetCurrCard(nmEvent.GetKillerMoves());//m_currMove_ = m_killerMoves_;

		// カードは作成せず、２枚目のカードを最後のカードとして覚えておきます。
		nmEvent.SetSeekbarTerminated(nmEvent.GetCurrCard() + 2);

	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {
		
		Move currCard = nmEvent.GetCurrCard()->m_move;

		//────────────────────
		// 進める☆
		//────────────────────
		nmEvent.GoNextCurCard();

		if (
			!currCard.IsNone() // 無いわけではない☆
			&&
			currCard != nmEvent.GetTranspositionTableMove() // トランスポジション・テーブル・ムーブでない☆
			&&
			(
				nmEvent.GetPos().GetTurn()==Color::Black
				?
				// 先手番が　スード・リーガル
				nmEvent.GetPos().MoveIsPseudoLegal<Color::Black,Color::White>(currCard, true)
				:
				// 後手版が　スード・リーガル
				nmEvent.GetPos().MoveIsPseudoLegal<Color::White,Color::Black>(currCard, true)
			)			
			&&
			// 移動先は、空きマスだ☆
			nmEvent.GetPos().GetPiece(currCard.To()) == N00_Empty
		){

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


extern N02___Killers g_phKillers;
