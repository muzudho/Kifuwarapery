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


class N13_PhTacticalMoves1 : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		nmEvent.SetTalonLastCard(
			// Ｎ０３（駒を取る手等）のカードを作成し、その最後のカードを覚えておきます。
			g_moveGenerator200.GenerateMoves_2(N03_CapturePlusPro, nmEvent.GetCurrCard(), nmEvent.GetPos())
		);

		nmEvent.ScoreCaptures();

	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		MoveStack* pBestcard = UtilMoveStack::PickBest(nmEvent.GetCurrCard(), nmEvent.GetTalonLastCard());

		nmEvent.GoNextCurCard();

		// todo: see が確実に駒打ちじゃないから、内部で駒打ちか判定してるのは少し無駄。
		if (
			pBestcard->m_move != nmEvent.GetTranspositionTableMove()
			&&
			nmEvent.GetCaptureThreshold() <
			(
				nmEvent.GetPos().GetTurn()==Color::Black
				?
				// 先手
				nmEvent.GetPos().GetSee1<Color::Black,Color::White>(pBestcard->m_move)
				:
				// 後手
				nmEvent.GetPos().GetSee1<Color::White,Color::Black>(pBestcard->m_move)
			)			
		) {

			//────────────────────────────────────────
			// 確定
			//────────────────────────────────────────
			pickedCard = pBestcard->m_move;
			return true;
		}

		//────────────────────────────────────────
		// 未確定
		//────────────────────────────────────────
		return false;
	};

};


extern N13_PhTacticalMoves1 g_phTacticalMoves1;
