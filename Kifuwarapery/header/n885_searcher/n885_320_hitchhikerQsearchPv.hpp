#pragma once


#include "n885_310_hitchhikerQsearchAbstract.hpp"


class HitchhikerQsearchPv : public HitchhikerQsearchAbstract {
public:


	virtual inline void DoAssert(
		ScoreIndex alpha,
		ScoreIndex beta
		) const override {
		//スルー☆！（＾ｑ＾）
	}

	virtual inline void SetOldAlpha(
		ScoreIndex& oldAlpha,
		ScoreIndex alpha
		) const override {
		oldAlpha = alpha;
	}

	virtual inline bool GetCondition01(
		const TTEntry** ppTtEntry,
		ScoreIndex beta,
		ScoreIndex ttScore
		) const override {
		// PVノードのとき☆（＾ｑ＾）
		return (*ppTtEntry)->GetType() == Bound::BoundExact;
	}

	virtual inline void SetAlpha(
		ScoreIndex& alpha,
		ScoreIndex bestScore
		) const override {
		// PVノードのとき☆（＾ｑ＾）
		if (alpha < bestScore) {
			alpha = bestScore;
		}
	}

	virtual inline void DoFutilityPruning01(
		bool& isContinue,
		bool& INCHECK,
		bool& givesCheck,
		Move& move,
		Move& ttMove,
		ScoreIndex& futilityScore,
		ScoreIndex& futilityBase,
		Position& pos,
		ScoreIndex& beta,
		ScoreIndex& bestScore,
		const Depth depth
		)const override {
		// スルーだぜ☆！（＾ｑ＾）
	}

	virtual inline void DoContinue01(
		bool& isContinue,
		bool& INCHECK,
		bool& evasionPrunable,
		Move& move,
		Move& ttMove,
		Position& pos
		)const {
		// スルーだぜ☆！（＾ｑ＾）
	}

	virtual inline void DoByNewScore(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		ScoreIndex& score,
		ScoreIndex& beta,
		ScoreIndex& alpha,
		Move& bestMove,
		Key& posKey,
		Flashlight** ppFlashlight,
		Depth ttDepth,
		Move move
		)const override {
		if (
			// PVノードのときは条件付きで別手続きがあるぜ☆（＾ｑ＾）
			score < beta
			) {
			alpha = score;
			bestMove = move;
		}
		else {
			// fail high
			rucksack.m_tt.Store(
				posKey,
				rucksack.ConvertScoreToTT(score, (*ppFlashlight)->m_ply),
				Bound::BoundLower,
				ttDepth,
				move,
				(*ppFlashlight)->m_staticEval
			);
			isReturnWithScore = true;
			returnScore = score;
			return;
			//return score;
		}
	}

	virtual inline Bound GetBound01(
		ScoreIndex& oldAlpha,
		ScoreIndex& bestScore
		)const override {
		return (oldAlpha < bestScore) ?
			Bound::BoundExact
			:
			Bound::BoundUpper
			;
	}

};