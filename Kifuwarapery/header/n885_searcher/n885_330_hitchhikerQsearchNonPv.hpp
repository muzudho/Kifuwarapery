#pragma once


#include "n885_310_hitchhikerQsearchAbstract.hpp"


class HitchhikerQsearchNonPv : public HitchhikerQsearchAbstract {
public:


	virtual inline void DoAssert(
		ScoreIndex alpha,
		ScoreIndex beta
		) const {
		assert(alpha == beta - 1);
	}

	virtual inline void SetOldAlpha(
		ScoreIndex& oldAlpha,
		ScoreIndex alpha
		) const {
		//スルー☆！（＾ｑ＾）
	}

	virtual inline bool GetCondition01(
		const TTEntry** ppTtEntry,
		ScoreIndex beta,
		ScoreIndex ttScore
		) const {
		// NonPVノードのとき☆（＾ｑ＾）
		return beta <= ttScore ?
			((*ppTtEntry)->GetType() & BoundLower)
			:
			((*ppTtEntry)->GetType() & BoundUpper);
	}

	virtual inline void SetAlpha(
		ScoreIndex& alpha,
		ScoreIndex bestScore
		) const {
		// スルーするぜ☆！（＾ｑ＾）
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
		)const {
		// 非PVノードのとき☆（＾ｑ＾）
		if (!INCHECK // 駒打ちは王手回避のみなので、ここで弾かれる。
			&& !givesCheck
			&& move != ttMove)
		{
			futilityScore =
				futilityBase + PieceScore::GetCapturePieceScore(pos.GetPiece(move.To()));
			if (move.IsPromotion()) {
				futilityScore += PieceScore::GetPromotePieceScore(move.GetPieceTypeFrom());
			}

			if (futilityScore < beta) {
				bestScore = std::max(bestScore, futilityScore);
				isContinue = true;
				return;
			}

			// todo: ＭｏｖｅＰｉｃｋｅｒ のオーダリングで SEE してるので、ここで SEE するの勿体無い。
			if (futilityBase < beta
				&& depth < Depth0
				&& pos.GetSee(move, beta - futilityBase) <= ScoreZero)
			{
				bestScore = std::max(bestScore, futilityBase);
				isContinue = true;
				return;
			}
		}
	}

	virtual inline void DoContinue01(
		bool& isContinue,
		bool& INCHECK,
		bool& evasionPrunable,
		Move& move,
		Move& ttMove,
		Position& pos
		)const {
		// PVノードでない場合☆（＾ｑ＾）
		if ((!INCHECK || evasionPrunable)
			&& move != ttMove
			&& (!move.IsPromotion() || move.GetPieceTypeFrom() != N01_Pawn)
			&& pos.GetSeeSign(move) < 0)
		{
			isContinue = true;
			return;
			//continue;
		}
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
		)const {
		// fail high
		rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(score, (*ppFlashlight)->m_ply), Bound::BoundLower,
			ttDepth, move, (*ppFlashlight)->m_staticEval);
		isReturnWithScore = true;
		returnScore = score;
		return;
		//return score;
	}

	virtual inline Bound GetBound01(
		ScoreIndex& oldAlpha,
		ScoreIndex& bestScore
		)const {
		return Bound::BoundUpper;
	}

};