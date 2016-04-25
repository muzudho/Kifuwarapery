#pragma once


#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


// SplitedNode = false
// IsRootNode = true
class NodetypeRoot : public NodetypeAbstract {
public:

	inline void GoSearch(Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		//────────────────────────────────────────────────────────────────────────────────
		// 探索☆？（＾ｑ＾）
		//────────────────────────────────────────────────────────────────────────────────
		Hitchhiker::Travel_885_510(	rucksack, NodeType::SplitedNodeRoot,
			pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

	inline const bool IsPvNode() const { return true; };

	virtual inline void DoStep1a(
		bool& isGotoSplitPointStart,
		int& moveCount,
		int& playedMoveCount,
		bool& inCheck,
		Position& pos,
		SplitedNode** ppSplitedNode,
		Flashlight** ppFlashlight,
		Move& bestMove,
		Move& threatMove,
		ScoreIndex& bestScore,
		Move& ttMove,
		Move& excludedMove,
		ScoreIndex& ttScore
		)const {
		// 非スプリット・ポイントはスルー☆！（＾ｑ＾）
	}

	virtual inline void DoStep2(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Position& pos,
		Rucksack& rucksack,
		Flashlight** ppFlashlight
		)const
	{
		// ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline void DoStep3(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Flashlight** ppFlashlight,
		ScoreIndex& alpha,
		ScoreIndex& beta
		)const {
		// ルートには無いぜ☆（＾ｑ＾）！
		//UNREACHABLE;
	}

	// ルートノードか、それ以外かで　値が分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep4x(
		Move& ttMove,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		Position& pos
		)const
	{
		ttMove = rucksack.m_rootMoves[rucksack.m_pvIdx].m_pv_[0];
	}

	virtual inline void DoStep4y(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		const Depth depth,
		ScoreIndex& ttScore,
		ScoreIndex& beta,
		Flashlight** ppFlashlight,
		Move& ttMove
		)const {
		// ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline void DoStep4z(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		bool& inCheck,
		Move& move,
		Position& pos,
		Flashlight** ppFlashlight,
		ScoreIndex& bestScore,
		Key& posKey,
		const Depth depth,
		Move& bestMove
		)const {
		// ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
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
		// 非スプリットポイントの場合
		return mp.GetNextMove_NonSplitedNode();
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
		// ルートはこういう感じ☆（＾ｑ＾）
		score = bestScore;
		singularExtensionNode = false;
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
			++moveCount;
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

	virtual inline void LockInStep13a(
		SplitedNode** ppSplitedNode
		) const
	{
		// 非スプリット・ポイントではスルー☆！（＾ｑ＾）
	}
	virtual inline void LockAndUpdateBestScoreInStep13a(
		SplitedNode** ppSplitedNode,
		ScoreIndex& bestScore
		) const {
		// 非スプリット・ポイントではスルー☆！（＾ｑ＾）
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

	virtual inline void UpdateAlphaInStep15(
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode
		) const {

		// 非スプリットノードではスルー☆！（＾ｑ＾）
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

	virtual inline void DoStep16a(
		bool& doFullDepthSearch,
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode
		)const {
		// 非スプリットノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline bool IsBetaLargeAtStep16c(
		ScoreIndex& score,
		ScoreIndex& beta
		) const {
		// ルートノードの場合☆（＾ｑ＾）
		return true;
	}

	virtual inline void DoStep18a(
		SplitedNode** ppSplitedNode,
		ScoreIndex& bestScore,
		ScoreIndex& alpha
		)const {
		// 非スプリット・ポイントはスルー☆！（＾ｑ＾）
	}

	// 非スプリット・ポイントの場合☆（＾ｑ＾）
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
			bestScore = score;

			if (alpha < score) {
				bestMove = move;

				// （＾ｑ＾）ＰＶノードの場合☆
				if (score < beta) {
					alpha = score;
				}
				else {
					isBreak = true;
					return;
				}
			}
		}

	}

	virtual inline bool GetReturnBeforeStep20() const {
		// 非スプリット・ポイントは　ステップ２０を実行する前に途中抜けはしないぜ☆（＾ｑ＾）
		return false;
	}

	virtual inline Bound GetBoundAtStep20(bool bestMoveExists) const {
		return bestMoveExists ? Bound::BoundExact : Bound::BoundUpper;
	}

};


extern NodetypeRoot g_nodetypeRoot;

