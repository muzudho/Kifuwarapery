#pragma once


#include "../n165_movStack/n165_400_move.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// キラー・ムーブ[0] でも [1] でもなければ☆
class N04_PhNonTacticalMoves1 : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		nmEvent.SetCurrCard(nmEvent.GetTalonLastCard());

		nmEvent.SetTalonLastCard(nmEvent.GetLastNonCapture());

		if (static_cast<Depth>(3 * OnePly) <= nmEvent.GetDepth()) {
			std::sort(nmEvent.GetCurrCard(), nmEvent.GetTalonLastCard(), std::greater<MoveStack>());
		}
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		Move currCard = nmEvent.GetCurrCard()->m_move;

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


extern N04_PhNonTacticalMoves1 g_phNonTacticalMoves1;
