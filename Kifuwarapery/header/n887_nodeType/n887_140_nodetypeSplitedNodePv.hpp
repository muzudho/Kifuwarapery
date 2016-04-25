#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


// PvNode = true
// SplitedNode = true
// RootNode = false
class NodetypeSplitedNodePv : public NodetypeAbstract {
public:

	inline void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		// スルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}


	// 非PVノードはassertをするぜ☆（＾ｑ＾）
	virtual inline void AssertBeforeStep1(
		ScoreIndex alpha,
		ScoreIndex beta
		) const {
		// PVノードはスルー☆！（＾ｑ＾）
		assert(alpha == beta - 1);
	}

	// ルートノードか、それ以外かで　値が分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep4x(
		Move& ttMove,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		Position& pos
		)const
	{
		ttMove = pTtEntry != nullptr ?
			UtilMoveStack::Move16toMove(pTtEntry->GetMove(), pos) :
			g_MOVE_NONE;
	}

	// PVノードの場合☆（＾ｑ＾）
	virtual inline bool GetConditionInStep4y(
		const TTEntry* pTtEntry,
		ScoreIndex& beta,
		ScoreIndex& ttScore
		) const {
		return pTtEntry->GetType() == Bound::BoundExact;
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep6(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		const Depth depth,
		ScoreIndex& eval,
		ScoreIndex& beta,
		Move& ttMove,
		Position& pos,
		Flashlight** ppFlashlight
		)const {
		// PVノードはスルー☆！（＾ｑ＾）
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep7(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Flashlight** ppFlashlight,
		const Depth depth,
		ScoreIndex& beta,
		ScoreIndex& eval
		)const {
		// PVノードはスルー☆！（＾ｑ＾）
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep8(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		Flashlight** ppFlashlight,
		const Depth depth,
		ScoreIndex& beta,
		ScoreIndex& eval,
		Position& pos,
		StateInfo& st,
		ScoreIndex& alpha,
		const bool cutNode,
		Move& threatMove
		)const {
		// PVノードはスルー☆！（＾ｑ＾）
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep9(
		bool& isReturnWithScore,
		Rucksack& rucksack,
		const Depth& depth,
		Flashlight** ppFlashlight,
		ScoreIndex& beta,
		Move& move,
		Position& pos,
		Move& ttMove,
		StateInfo& st,
		ScoreIndex& score,
		const bool cutNode
		)const {
		// 非PVノードはスルー☆！（＾ｑ＾）
	}

	// PVノードか、そうでないかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void DoStep10(
		const Depth depth,
		Move& ttMove,
		bool& inCheck,
		ScoreIndex& beta,
		Flashlight** ppFlashlight,
		Rucksack& rucksack,
		Position& pos,
		ScoreIndex& alpha,
		const TTEntry** ppTtEntry,//セットされるぜ☆
		Key& posKey
		)const
	{
		// internal iterative deepening
		if (
			// PVノードの場合、５倍☆
			(5 * OnePly) <= depth
			&& ttMove.IsNone()
			)
		{
			//const Depth d = depth - 2 * OnePly - (PVNode ? Depth0 : depth / 4);
			// PVノードの場合☆
			const Depth d = depth - 2 * OnePly;

			(*ppFlashlight)->m_skipNullMove = true;

			//────────────────────────────────────────────────────────────────────────────────
			// 探索☆？（＾ｑ＾）
			//────────────────────────────────────────────────────────────────────────────────
			// PVノードの場合☆
			Hitchhiker::Travel_885_510(rucksack, NodeType::N01_PV, pos, (*ppFlashlight), alpha, beta, d, true);

			(*ppFlashlight)->m_skipNullMove = false;

			(*ppTtEntry) = rucksack.m_tt.Probe(posKey);
			ttMove = ((*ppTtEntry) != nullptr ?
				UtilMoveStack::Move16toMove((*ppTtEntry)->GetMove(), pos) :
				g_MOVE_NONE);
		}
	}

	virtual inline ScoreIndex GetBetaAtStep11(
		ScoreIndex beta
		) const {
		// PVノードの場合☆（＾ｑ＾）
		return -ScoreIndex::ScoreInfinite;
	}

	virtual inline Move GetMoveAtStep11(
		NextmoveEvent& mp
		) const {
		// スプリットポイントの場合
		return mp.GetNextMove_SplitedNode();
	};

	virtual inline void DoStep11A_BeforeLoop_SplitPointStart(
		Move& ttMove,
		const Depth depth,
		ScoreIndex& score,
		ScoreIndex& bestScore,
		bool& singularExtensionNode,
		Move& excludedMove,
		const TTEntry* pTtEntry
		)const
	{
		// ルートでない場合はこういう感じ☆（＾ｑ＾）
		score = bestScore;
		singularExtensionNode = false;
	}

	virtual inline void DoStep11Bb_LoopHeader(
		bool& isContinue,
		const Rucksack& rucksack,
		const Move& move
		)const {
		// 非ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline void DoStep11Bc_LoopHeader(
		Rucksack& rucksack,
		int& moveCount
		) const {
		// 非ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// スプリット・ポイントかどうかで変わる手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep11Bb_LoopHeader(
		bool& isContinue,
		Position& pos,
		Move& move,
		const CheckInfo& ci,
		int& moveCount,
		SplitedNode** ppSplitedNode
		) const {
			if (!pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
				isContinue = true;
				return;
			}
			moveCount = ++(*ppSplitedNode)->m_moveCount;
			(*ppSplitedNode)->m_mutex.unlock();
	}

	virtual inline void DoStep13a(
		bool& isContinue,
		Rucksack& rucksack,
		bool& captureOrPawnPromotion,
		bool& inCheck,
		bool& dangerous,
		ScoreIndex& bestScore,
		Move& move,
		Move& ttMove,
		const Depth depth,
		int& moveCount,
		Move& threatMove,
		Position& pos,
		SplitedNode** ppSplitedNode,
		Depth& newDepth,
		Flashlight** ppFlashlight,
		ScoreIndex& beta
		) const {
		// PVノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// PVノードか、そうでないかで変わるぜ☆！（＾ｑ＾）
	virtual inline const Depth GetPredictedDepthInStep13a(
		Depth& newDepth,
		const Depth depth,
		int& moveCount
		) const {
		// PVノードのとき
		return newDepth - g_reductions.DoReduction_PvNode(depth, moveCount);
	}

	virtual inline void DoStep13b(
		Position& pos,
		Move& move,
		const CheckInfo& ci,
		int& moveCount,
		bool& isContinue
		) const {
		// ルートノード、スプリットポイントはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// スプリット・ポイントか、PVノードかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void DoStep13c(
		bool& isContinue,
		Rucksack& rucksack,
		bool& captureOrPawnPromotion,
		bool& inCheck,
		bool& dangerous,
		ScoreIndex& bestScore,
		Move& move,
		Move& ttMove,
		const Depth depth,
		int& moveCount,
		Move& threatMove,
		Position& pos,
		SplitedNode** ppSplitedNode,
		Depth& newDepth,
		Flashlight** ppFlashlight,
		ScoreIndex& beta,
		const CheckInfo& ci,
		bool& isPVMove,
		int& playedMoveCount,
		Move movesSearched[64]
		)const {

		// PVノードだぜ☆！（＾ｑ＾）
		isPVMove = (moveCount == 1);
		(*ppFlashlight)->m_currentMove = move;
	}

	virtual inline void DoStep13d(
		bool& captureOrPawnPromotion,
		int& playedMoveCount,
		Move movesSearched[64]
		) const {

		// スプリットポイントではスルー☆！（＾ｑ＾）
	}

	// Pvノードかどうかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void SetReductionInStep15(
		Flashlight** ppFlashlight,
		const Depth depth,
		int& moveCount,
		const bool cutNode
		) const {
		// Pvノードのとき☆！（＾ｑ＾）
		(*ppFlashlight)->m_reduction = g_reductions.DoReduction_PvNode(depth, moveCount);
	}

	virtual inline bool IsBetaLargeAtStep16c(
		ScoreIndex& score,
		ScoreIndex& beta
		) const {
		// 非ルートノードの場合☆（＾ｑ＾）
		return score < beta;
	}

	virtual inline void DoStep18b(
		Rucksack& rucksack,
		Move& move,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		Position& pos
		) const {
		// 非ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// スプリット・ポイントの場合☆（＾ｑ＾）
	virtual inline void DoStep18c(
		bool& isBreak,
		Rucksack& rucksack,
		Move& move,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		Position& pos,
		ScoreIndex& bestScore,
		SplitedNode** ppSplitedNode,
		Move& bestMove,
		ScoreIndex& beta
		)const {

		if (bestScore < score) {
			bestScore = (*ppSplitedNode)->m_bestScore = score;

			if (alpha < score) {
				bestMove = (*ppSplitedNode)->m_bestMove = move;

				// （＾ｑ＾）PVノードの場合☆
				if (score < beta) {
					alpha = (*ppSplitedNode)->m_alpha = score;
				}
				else {
					// fail high
					(*ppSplitedNode)->m_cutoff = true;
					isBreak = true;
					return;
				}
			}
		}

	}

	inline void DoStep19(
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
		)const {
		// スプリット・ポイントはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline bool GetReturnBeforeStep20() const {
		// スプリット・ポイントは　ステップ２０を実行する前に途中抜けするぜ☆（＾ｑ＾）
		return true;
	}

	inline void DoStep20(
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
		)const {
		// スプリット・ポイントはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	inline Bound GetBoundAtStep20(bool bestMoveExists) const {
		return bestMoveExists ? Bound::BoundExact : Bound::BoundUpper;
	}

};


extern NodetypeSplitedNodePv g_nodetypeSplitedNodePv;

