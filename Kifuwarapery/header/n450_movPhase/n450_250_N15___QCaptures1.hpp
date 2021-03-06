﻿#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_025_utilMoveStack.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// 駒を取り返す手☆
// すぐ確定
class N15___QCaptures1 : public MovePhaseAbstract {
public:

	// virtual の派生クラスなので template化はできないぜ☆（＾ｑ＾）
	// NextmoveEvent::Do01_GoNextPhase() で使われるぜ☆
	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 山札の底（最初）のカードに、カーソルを合わせます。
		nmEvent.BackToHome_CurrCard();

		const Color us = nmEvent.GetPos().GetTurn();

		// リキャプチャ（駒を取り返す手等）のカードを作成し、その最後のカードを覚えておきます。
		MoveStack* pNewTalon =
			us == Color::Black
			?
			g_moveGenerator200.GenerateMoves_recapture<Color::Black, Color::White>(nmEvent.GetTalonFirstCard(), nmEvent.GetPos(), nmEvent.GetRecaptureSquare())
			:
			g_moveGenerator200.GenerateMoves_recapture<Color::White, Color::Black>(nmEvent.GetTalonFirstCard(), nmEvent.GetPos(), nmEvent.GetRecaptureSquare())
			;//<Recapture>

		nmEvent.SetSeekbarTerminated(pNewTalon);

		nmEvent.DoScoreing_Captures();
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		Move bestcard = UtilMoveStack::PickBest_AndSort(nmEvent.GetCurrCard(), nmEvent.GetSeekbarTerminated())->m_move;

		//────────────────────
		// 進める☆
		//────────────────────
		nmEvent.GoNextCurCard();

		assert(bestcard.To() == nmEvent.GetRecaptureSquare());

		//────────────────────────────────────────
		// 確定　（Ｎｏｎｅの場合もあり☆？）
		//────────────────────────────────────────
		pickedCard = bestcard;
		return true;
	};

};


extern N15___QCaptures1 g_phQCaptures1;
