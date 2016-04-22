#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp" // Searcherと持ち合い
class SearchEvent1;

#include "../n885_searcher/n885_500_rucksack.hpp"//FIXME:
//class Rucksack;


class NodetypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	static void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) {};

	virtual const bool IsPvNode() const = 0;
	virtual const bool IsSplitedNode() const = 0;
	virtual const bool IsRootNode() const = 0;

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
			rucksack.m_tt.Store(posKey, rucksack.scoreToTT(bestScore, ss.m_ply), BoundLower, depth,
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
			rucksack.m_tt.Store(posKey, rucksack.scoreToTT(bestScore, ss.m_ply),
				((this->IsPvNode() && !bestMove.IsNone()) ? BoundExact : BoundUpper),
				depth, bestMove, ss.m_staticEval);
		}

	}
};


