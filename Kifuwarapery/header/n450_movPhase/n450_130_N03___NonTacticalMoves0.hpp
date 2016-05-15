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
// 駒を取らない手（盤上、持ち駒）の指し手を　山札に追加します。
class N03___NonTacticalMoves0 : public MovePhaseAbstract {
public:

	void Do02_ExtendTalon(NextmoveEvent& nmEvent) override {

		// 山札の底（最初）のカードに、カーソルを合わせます。
		nmEvent.BackToHome_CurrCard(); 

		//────────────────────────────────────────────────────────────────────────────────
		// 駒を取らない手（盤上）
		//────────────────────────────────────────────────────────────────────────────────
		// Ｎ０４（駒を取らない手・盤上）のカードを作成し、その最後のカードを覚えておきます。
		MoveStack* pNewTalon_1 = g_moveGenerator200.GenerateMoves_2(N04_NonCaptureMinusPro, nmEvent.GetCurrCard(), nmEvent.GetPos());
		nmEvent.SetSeekbarTerminated(pNewTalon_1);
		
		nmEvent.DoScoreing_NonCapturesMinusPro<false>();// 点数付け
		nmEvent.GoToTopTalon_CurrCard();// 駒を取らない盤上の手は、すっとばすぜ☆

		//────────────────────────────────────────────────────────────────────────────────
		// 駒を取らない手（持ち駒）
		//────────────────────────────────────────────────────────────────────────────────

		// このあとと、そのあとで、phNonTacticalMoves1 で、ラスト・ムーブ　として使う☆
		MoveStack* pNewTalon_2 = g_moveGenerator200.GenerateMoves_2(N02_Drop, nmEvent.GetCurrCard(), nmEvent.GetPos());
		// 駒を打つ手のカードを作成し、その最後のカードを覚えておきます。
		nmEvent.SetSeekbarTerminated(pNewTalon_2);
		nmEvent.SetLastNonCaputre(pNewTalon_2);

		nmEvent.DoScoreing_NonCapturesMinusPro<true>();// 点数付け

		//────────────────────────────────────────────────────────────────────────────────
		//
		//────────────────────────────────────────────────────────────────────────────────

		nmEvent.BackToHome_CurrCard();// 山札の底（最初）☆
		// 「駒を取らない手」の山札全体について、プラスのスコアを、先頭近くに移動させるぜ☆
		MoveStack* pSortedTalon_3 = std::partition(nmEvent.GetCurrCard(), nmEvent.Get_LastNonCapture(), HasPositiveScore());
		nmEvent.SetSeekbarTerminated(pSortedTalon_3);

		// 点数が高い順か☆？ソートするぜ☆
		// 要素数は10個くらいまでであることが多い。要素数が少ないので、insertionSort() を使用する。
		UtilMove01::InsertionSort<MoveStack*, true>(nmEvent.GetCurrCard(), nmEvent.GetSeekbarTerminated());
		// なんで２回ソートしたのか☆？（＾ｑ＾）？
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


extern N03___NonTacticalMoves0 g_phNonTacticalMoves0;
