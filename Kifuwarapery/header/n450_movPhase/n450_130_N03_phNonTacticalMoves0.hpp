#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../n440_movStack/n440_400_hasPositiveScore.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "n450_070_movePhaseAbstract.hpp"


class NextmoveEvent;


// キラー・ムーブ[0] でも [1] でもなければ☆
class N03_PhNonTacticalMoves0 : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {
		nmEvent.SetTalonLastCard(
			// Ｎ０４（駒を取らない手等）のカードを作成し、その最後のカードを覚えておきます。
			g_moveGenerator200.GenerateMoves_2(N04_NonCaptureMinusPro, nmEvent.GetCurrCard(), nmEvent.GetPos())
		);

		nmEvent.ScoreNonCapturesMinusPro<false>();
		nmEvent.SetCurrCard(nmEvent.GetTalonLastCard());

		// phNonTacticalMoves1 で、ラスト・ムーブ　として使う☆
		nmEvent.SetTalonLastCardAndLastNonCaputre(
			g_moveGenerator200.GenerateMoves_2(N02_Drop, nmEvent.GetCurrCard(), nmEvent.GetPos())
		);
		nmEvent.ScoreNonCapturesMinusPro<true>();

		nmEvent.SetCurrCard(nmEvent.GetTalonFirstCard());

		nmEvent.SetTalonLastCard(std::partition(nmEvent.GetCurrCard(), nmEvent.GetLastNonCapture(), HasPositiveScore()));

		// 要素数は10個くらいまでであることが多い。要素数が少ないので、insertionSort() を使用する。
		UtilMove01::InsertionSort<MoveStack*, true>(nmEvent.GetCurrCard(), nmEvent.GetTalonLastCard());
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


extern N03_PhNonTacticalMoves0 g_phNonTacticalMoves0;
