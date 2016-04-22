#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp" // Searcherと持ち合い
#include "../n640_searcher/n640_500_reductions.hpp"
#include "../n640_searcher/n640_510_futilityMargins.hpp"
#include "../n640_searcher/n640_520_futilityMoveCounts.hpp"

#include "../n885_searcher/n885_500_rucksack.hpp"//FIXME:
//class Rucksack;

using namespace std;


class NodetypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	virtual void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) const = 0;

	virtual const bool IsPvNode() const = 0;
	virtual const bool IsSplitedNode() const = 0;
	virtual const bool IsRootNode() const = 0;

	void DoStep13(
		bool& isContinue,
		Rucksack& rucksack,
		bool& captureOrPawnPromotion,
		bool& inCheck,
		bool& dangerous,
		ScoreIndex& bestScore,
		Move& move,
		Move& ttMove,
		const Depth& depth,
		int& moveCount,
		Move& threatMove,
		Position& pos,
		SplitedNode* splitedNode,
		Depth& newDepth,
		Flashlight& ss,
		ScoreIndex& beta,
		const CheckInfo& ci,
		bool& isPVMove,
		int& playedMoveCount,
		Move movesSearched[64]
		) {

		// futility pruning
		if (!this->IsPvNode()
			&& !captureOrPawnPromotion
			&& !inCheck
			&& !dangerous
			//&& move != ttMove // 次の行がtrueならこれもtrueなので条件から省く。
			&& ScoreMatedInMaxPly < bestScore)
		{
			assert(move != ttMove);
			// move count based pruning
			if (depth < 16 * OnePly
				&& g_futilityMoveCounts.m_futilityMoveCounts[depth] <= moveCount
				&& (threatMove.IsNone() || !rucksack.refutes(pos, move, threatMove)))
			{
				if (this->IsSplitedNode()) {
					splitedNode->m_mutex.lock();
				}
				isContinue = true;
				return;
			}

			// score based pruning
			const Depth predictedDepth = newDepth - g_reductions.reduction(this->IsPvNode(), depth, moveCount);
			// gain を 2倍にする。
			const ScoreIndex futilityScore = ss.m_staticEval + g_futilityMargins.GetFutilityMargin(predictedDepth, moveCount)
				+ 2 * rucksack.m_gains.GetValue(move.IsDrop(), ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(pos.GetTurn(), move.GetPieceTypeFromOrDropped()), move.To());

			if (futilityScore < beta) {
				bestScore = std::max(bestScore, futilityScore);
				if (this->IsSplitedNode()) {
					splitedNode->m_mutex.lock();
					if (splitedNode->m_bestScore < bestScore) {
						splitedNode->m_bestScore = bestScore;
					}
				}
				isContinue = true;
				return;
			}

			if (predictedDepth < 4 * OnePly
				&& pos.GetSeeSign(move) < ScoreZero)
			{
				if (this->IsSplitedNode()) {
					splitedNode->m_mutex.lock();
				}
				isContinue = true;
				return;
			}
		}

		// RootNode, SPNode はすでに合法手であることを確認済み。
		if (!this->IsRootNode() && !this->IsSplitedNode() && !pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
			--moveCount;
			isContinue = true;
			return;
		}

		isPVMove = (this->IsPvNode() && moveCount == 1);
		ss.m_currentMove = move;
		if (!this->IsSplitedNode() && !captureOrPawnPromotion && playedMoveCount < 64) {
			movesSearched[playedMoveCount++] = move;
		}

	}

	void DoStep18(
		bool& isBreak,
		Rucksack& rucksack,
		Move& move,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		Position& pos,
		ScoreIndex& bestScore,
		SplitedNode* splitedNode,
		Move& bestMove,
		ScoreIndex& beta
		) {

		if (this->IsRootNode()) {
			RootMove& rm = *std::find(rucksack.m_rootMoves.begin(), rucksack.m_rootMoves.end(), move);
			if (isPVMove || alpha < score) {
				// PV move or new best move
				rm.m_score_ = score;
#if defined BISHOP_IN_DANGER
				if ((bishopInDangerFlag == BlackBishopInDangerIn28 && GetMove.ToCSA() == "0082KA")
					|| (bishopInDangerFlag == WhiteBishopInDangerIn28 && GetMove.ToCSA() == "0028KA")
					|| (bishopInDangerFlag == BlackBishopInDangerIn78 && GetMove.ToCSA() == "0032KA")
					|| (bishopInDangerFlag == WhiteBishopInDangerIn78 && GetMove.ToCSA() == "0078KA"))
				{
					rm.m_score_ -= m_engineOptions["Danger_Demerit_Score"];
				}
#endif
				rm.ExtractPvFromTT(pos);

				if (!isPVMove) {
					++rucksack.m_bestMoveChanges;
				}
			}
			else {
				rm.m_score_ = -ScoreInfinite;
			}
		}

		if (bestScore < score) {
			bestScore = (this->IsSplitedNode() ? splitedNode->m_bestScore = score : score);

			if (alpha < score) {
				bestMove = (this->IsSplitedNode() ? splitedNode->m_bestMove = move : move);

				if (this->IsPvNode() && score < beta) {
					alpha = (this->IsSplitedNode() ? splitedNode->m_alpha = score : score);
				}
				else {
					// fail high
					if (this->IsSplitedNode()) {
						splitedNode->m_cutoff = true;
					}
					isBreak = true;
					return;
				}
			}
		}

	}

	void DoStep19(
		bool& isBreak,
		Rucksack& rucksack,
		const Depth& depth,
		Military* pThisThread,
		ScoreIndex& bestScore,
		ScoreIndex& beta,
		Position& pos,
		Flashlight& ss,
		ScoreIndex& alpha,
		Move& bestMove,
		Move& threatMove,
		int& moveCount,
		NextmoveEvent& mp,
		NodeType NT,
		const bool cutNode
		){
		if (!this->IsSplitedNode()
			&& rucksack.m_ownerHerosPub.GetMinSplitDepth() <= depth
			&& rucksack.m_ownerHerosPub.GetAvailableSlave(pThisThread)
			&& pThisThread->m_splitedNodesSize < g_MaxSplitedNodesPerThread)
		{
			assert(bestScore < beta);
			pThisThread->ForkNewFighter<Rucksack::FakeSplit>(
				pos, &ss, alpha, beta, bestScore, bestMove,
				depth, threatMove, moveCount, mp, NT, cutNode
				);
			if (beta <= bestScore) {
				isBreak = true;
				return;
			}
		}

	}

	void DoStep20(
		Rucksack& rucksack,
		ScoreIndex& alpha,
		Flashlight& ss,
		ScoreIndex& bestScore,
		int& playedMoveCount,
		ScoreIndex& beta,
		Key& posKey,
		const Depth depth,
		Move& bestMove,
		bool& inCheck,
		Position& pos,
		Move movesSearched[64]
	) {
		if (bestScore == -ScoreInfinite) {
			assert(playedMoveCount == 0);
			bestScore = alpha;
		}

		if (beta <= bestScore) {
			// failed high
			rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, ss.m_ply), BoundLower, depth,
				bestMove, ss.m_staticEval);

			if (!bestMove.IsCaptureOrPawnPromotion() && !inCheck) {
				if (bestMove != ss.m_killers[0]) {
					ss.m_killers[1] = ss.m_killers[0];
					ss.m_killers[0] = bestMove;
				}

				const ScoreIndex bonus = static_cast<ScoreIndex>(depth * depth);
				const Piece pc1 = ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(pos.GetTurn(), bestMove.GetPieceTypeFromOrDropped());
				rucksack.m_history.Update(bestMove.IsDrop(), pc1, bestMove.To(), bonus);

				for (int i = 0; i < playedMoveCount - 1; ++i) {
					const Move m = movesSearched[i];
					const Piece pc2 = ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(pos.GetTurn(), m.GetPieceTypeFromOrDropped());
					rucksack.m_history.Update(m.IsDrop(), pc2, m.To(), -bonus);
				}
			}
		}
		else {
			// failed low or PV search
			rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, ss.m_ply),
				((this->IsPvNode() && !bestMove.IsNone()) ? BoundExact : BoundUpper),
				depth, bestMove, ss.m_staticEval);
		}

	}
};


