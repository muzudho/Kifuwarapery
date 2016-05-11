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


// 駒を取り返す手☆
// すぐ確定
class N15_PhQCaptures1 : public MovePhaseAbstract {
public:

	// virtual の派生クラスなので template化はできないぜ☆（＾ｑ＾）
	// NextmoveEvent::Do01_GoNextPhase() で使われるぜ☆
	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		const Color us = nmEvent.GetPos().GetTurn();

		nmEvent.SetTalonLastCard(
			// リキャプチャ（駒を取り返す手等）のカードを作成し、その最後のカードを覚えておきます。
			us == Color::Black
			?
			g_moveGenerator200.GenerateMoves_recapture<Color::Black, Color::White>(nmEvent.GetTalonFirstCard(), nmEvent.GetPos(), nmEvent.GetRecaptureSquare())
			:
			g_moveGenerator200.GenerateMoves_recapture<Color::White, Color::Black>(nmEvent.GetTalonFirstCard(), nmEvent.GetPos(), nmEvent.GetRecaptureSquare())
			);//<Recapture>

		nmEvent.ScoreCaptures();
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		Move bestcard = UtilMoveStack::PickBest(nmEvent.GetCurrCard(), nmEvent.GetTalonLastCard())->m_move;

		nmEvent.GoNextCurCard();

		assert(bestcard.To() == nmEvent.GetRecaptureSquare());

		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅの場合もあり☆？）
		//────────────────────────────────────────
		pickedCard = bestcard;
		return true;
	};

};


extern N15_PhQCaptures1 g_phQCaptures1;
