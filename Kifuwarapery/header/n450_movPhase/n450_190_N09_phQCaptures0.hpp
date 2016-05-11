#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_025_utilMoveStack.hpp"
#include "../n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// 駒を取る手☆
// トランスポジション・テーブル・ムーブ　でなければ確定☆
class N09_PhQCaptures0 : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 駒を取る手を読む☆？
		nmEvent.SetTalonLastCard(
			// Ｎ０３（駒を取る手等）のカードを作成し、その最後のカードを覚えておきます。
			g_moveGenerator200.GenerateMoves_2(N03_CapturePlusPro, nmEvent.GetTalonFirstCard(), nmEvent.GetPos())
		);

		nmEvent.ScoreCaptures();
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		// ベストムーブを取得☆
		Move bestCard = UtilMoveStack::PickBest(nmEvent.GetCurrCard(), nmEvent.GetTalonLastCard())->m_move;

		nmEvent.GoNextCurCard();

		if (bestCard != nmEvent.GetTranspositionTableMove()) {

			//────────────────────────────────────────
			// 確定
			//────────────────────────────────────────
			pickedCard = bestCard;
			return true;
		}

		//────────────────────────────────────────
		// 未確定
		//────────────────────────────────────────
		return false;
	};

};


extern N09_PhQCaptures0 g_phQCaptures0;
