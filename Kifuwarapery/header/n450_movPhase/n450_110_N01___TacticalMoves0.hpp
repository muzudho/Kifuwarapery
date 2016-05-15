#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_025_utilMoveStack.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// 閾値を使った、駒の取り合い☆？
// 駒を取る手☆
class N01___TacticalMoves0 : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 山札の底（最初）のカードに、カーソルを合わせます。
		nmEvent.BackToHome_CurrCard();

		// Ｎ０３（駒を取る手等）のカードを作成し、その最後のカードを覚えておきます。
		MoveStack* pNewTalon = g_moveGenerator200.GenerateMoves_2(N03_CapturePlusPro, nmEvent.GetCurrCard(), nmEvent.GetPos());
		nmEvent.SetSeekbarTerminated(pNewTalon);

		nmEvent.DoScoreing_Captures();//点数付け
	}

	bool Do03_PickCard_OrNextCard(Move& pickedCard, NextmoveEvent& nmEvent) const override {

		// 一番高い、駒を取る手☆？
		MoveStack* pBestcard = UtilMoveStack::PickBest_AndSort(nmEvent.GetCurrCard(), nmEvent.GetSeekbarTerminated());
		
		//────────────────────
		// 進める☆
		//────────────────────
		nmEvent.GoNextCurCard();

		if (pBestcard->m_move != nmEvent.GetTranspositionTableMove()) {
			assert(nmEvent.GetCaptureThreshold() <= 0);

			if (
				// 閾値より、SEE値の高い手☆
				nmEvent.GetCaptureThreshold() <=
				(
					nmEvent.GetPos().GetTurn()==Color::Black
					?
					// 先手番（駒の取り合い☆？）
					nmEvent.GetPos().GetSee1<Color::Black,Color::White>(pBestcard->m_move)
					:
					// 後手番（駒の取り合い☆？）
					nmEvent.GetPos().GetSee1<Color::White,Color::Black>(pBestcard->m_move)
				)
			) {
				// 駒取りの閾値を超えていれば☆

				//────────────────────────────────────────
				// 確定
				//────────────────────────────────────────
				pickedCard = pBestcard->m_move;
				return true;
			}

			// SEE値が低かったカードのうち、
			// 山札の許容量空間の　後ろから SEE の点数が高い順に並ぶように入れている☆
			nmEvent.GetBottom_SkyTalon()->m_move = pBestcard->m_move;
			nmEvent.DownBottom_SkyTalon();
		}

		//────────────────────────────────────────
		// 未確定
		//────────────────────────────────────────
		return false;
	};

};


extern N01___TacticalMoves0 g_phTacticalMoves0;
