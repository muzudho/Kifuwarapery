﻿#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n640_searcher/n640_500_reductions.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"
#include "../n887_nodeType/n887_120_nodetypeNonPv.hpp"


// PvNode = false
// SplitedNode = true
// RootNode = false
class NodetypeSplitedNodeNonPv : public NodetypeAbstract {
public:

	inline void GoSearch_AsSplitedNode(
		Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp
		) const override {
		// スルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}


	inline void DoStep1c(
		Military** ppThisThread,
		const Flashlight* pFlashlight
		)const override {
		// PVノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// ルートノードか、それ以外かで　値が分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep4x(
		Move& ttMove,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		Position& pos
		)const override
	{
		ttMove = pTtEntry != nullptr ?
			UtilMoveStack::Move16toMove(pTtEntry->GetMove(), pos) :
			g_MOVE_NONE;
	}

	// 非PVノードの場合☆（＾ｑ＾）
	virtual inline bool GetConditionInStep4y(
		const TTEntry* pTtEntry,
		ScoreIndex& beta,
		ScoreIndex& ttScore
		) const override {
		return  beta <= ttScore ?
			(pTtEntry->GetType() & Bound::BoundLower)
			:
			(pTtEntry->GetType() & Bound::BoundUpper);
	}

	virtual inline ScoreIndex GotoTheAdventure01InStep8_NonPV(
		Rucksack& rucksack,
		Position& pos,
		Flashlight** ppFlashlight,
		ScoreIndex& beta,
		ScoreIndex& alpha,
		const Depth depth,
		const Depth reduction,
		const bool cutNode
		) const override  {

		// スプリット・ポイントではない方の関数を使うぜ☆（＾ｑ＾）
		return - g_NODETYPE_NON_PV.GoToTheAdventure_new(
			rucksack,
			pos,
			(*ppFlashlight) + 1,
			-beta,
			-alpha,
			depth - reduction,
			!cutNode
			);
	};
	virtual inline ScoreIndex GotoTheAdventure02InStep8_NonPV(
		Rucksack& rucksack,
		Position& pos,
		Flashlight** ppFlashlight,
		ScoreIndex& beta,
		ScoreIndex& alpha,
		const Depth depth,
		const Depth reduction,
		const bool cutNode
		) const override {

		// スプリット・ポイントではない方の関数を使うぜ☆（＾ｑ＾）
		return g_NODETYPE_NON_PV.GoToTheAdventure_new(
			rucksack,
			pos,
			(*ppFlashlight),
			alpha,
			beta,
			depth - reduction,
			false
			);
	};

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	// N02_NonPV扱いで実行する関数があるぜ、なんだこれ☆（＾ｑ＾）
	virtual inline ScoreIndex GotoTheAdventure03InStep9_NonPV(
		Rucksack& rucksack,
		Position& pos,
		Flashlight** ppFlashlight,
		const ScoreIndex rbeta,
		const Depth rdepth,
		const bool cutNode
		) const override {
		return -g_NODETYPE_NON_PV.GoToTheAdventure_new(
			rucksack,
			pos,
			(*ppFlashlight) + 1,
			-rbeta,
			-rbeta + 1,
			rdepth,
			!cutNode
			);
	};

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
		)const override
	{
		// internal iterative deepening
		if (
			// 非PVノードの場合、８倍☆
			(8 * OnePly) <= depth
			&& ttMove.IsNone()
			// 非PVノードの場合、さらに条件☆
			&& (
				!inCheck && beta <= (*ppFlashlight)->m_staticEval + static_cast<ScoreIndex>(256)
				)
			)
		{
			//const Depth d = depth - 2 * OnePly - (PVNode ? Depth0 : depth / 4);
			// 非PVノードの場合☆
			const Depth d = depth / 2;

			(*ppFlashlight)->m_skipNullMove = true;

			//────────────────────────────────────────────────────────────────────────────────
			// 探索☆？（＾ｑ＾）
			//────────────────────────────────────────────────────────────────────────────────
			// 非PVノードの場合☆
			g_NODETYPE_NON_PV.GoToTheAdventure_new(
				rucksack, pos, (*ppFlashlight), alpha, beta, d, true);

			(*ppFlashlight)->m_skipNullMove = false;

			(*ppTtEntry) = rucksack.m_tt.Probe(posKey);
			ttMove = ((*ppTtEntry) != nullptr ?
				UtilMoveStack::Move16toMove((*ppTtEntry)->GetMove(), pos) :
				g_MOVE_NONE);
		}
	}

	virtual inline ScoreIndex GetBetaAtStep11(
		ScoreIndex beta
		) const override {
		// 非PVノードの場合☆（＾ｑ＾）
		return beta;
	}

	virtual inline Move GetMoveAtStep11(
		NextmoveEvent& mp
		) const override {
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
		)const override
	{
		// ルートでない場合はこういう感じ☆（＾ｑ＾）
		score = bestScore;
		singularExtensionNode = false;
	}

	virtual inline void DoStep11Bb_LoopHeader(
		bool& isContinue,
		const Rucksack& rucksack,
		const Move& move
		)const override {
		// 非ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline void DoStep11Bc_LoopHeader(
		Rucksack& rucksack,
		int& moveCount
		) const override {
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
		) const override {
			if (!pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
				isContinue = true;
				return;
			}
			moveCount = ++(*ppSplitedNode)->m_moveCount;
			(*ppSplitedNode)->m_mutex.unlock();
	}

	// PVノードか、そうでないかで変わるぜ☆！（＾ｑ＾）
	virtual inline const Depth GetPredictedDepthInStep13a(
		Depth& newDepth,
		const Depth depth,
		int& moveCount
		) const override {
		// 非PVノードのとき
		return newDepth - g_reductions.DoReduction_NotPvNode(depth, moveCount);
	}

	virtual inline void DoStep13b(
		Position& pos,
		Move& move,
		const CheckInfo& ci,
		int& moveCount,
		bool& isContinue
		) const override {
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
		)const override {

		// 非PVノードだぜ☆（＾ｑ＾）
		isPVMove = false;
		(*ppFlashlight)->m_currentMove = move;
	}

	virtual inline void DoStep13d(
		bool& captureOrPawnPromotion,
		int& playedMoveCount,
		Move movesSearched[64],
		Move& move
		) const override {

		// スプリットポイントではスルー☆！（＾ｑ＾）
	}

	// Pvノードかどうかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void SetReductionInStep15(
		Flashlight** ppFlashlight,
		const Depth depth,
		int& moveCount,
		const bool cutNode
		) const override {
		// 非Pvノードのとき☆！（＾ｑ＾）
		(*ppFlashlight)->m_reduction = g_reductions.DoReduction_NotPvNode(depth, moveCount);
		if (cutNode) {
			(*ppFlashlight)->m_reduction += Depth::OnePly;
		}
	}

	virtual inline void DoStep16c(
		Rucksack& rucksack,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		ScoreIndex& beta,
		Depth& newDepth,
		bool& givesCheck,
		Position& pos,
		Flashlight** ppFlashlight
		)const override {
		// 非PVノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline bool IsBetaLargeAtStep16c(
		ScoreIndex& score,
		ScoreIndex& beta
		) const override {
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
		) const override {
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
		)const override {

		if (bestScore < score) {
			bestScore = (*ppSplitedNode)->m_bestScore = score;

			if (alpha < score) {
				bestMove = (*ppSplitedNode)->m_bestMove = move;

				// PVノードではない場合☆（＾ｑ＾）
				// fail high
				(*ppSplitedNode)->m_cutoff = true;
				isBreak = true;
				return;
			}
		}

	}

	inline void DoStep19(
		bool& isBreak,
		Rucksack& rucksack,
		const Depth depth,
		Military** ppThisThread,
		ScoreIndex& bestScore,
		ScoreIndex& beta,
		Position& pos,
		Flashlight** ppFlashlight,
		ScoreIndex& alpha,
		Move& bestMove,
		Move& threatMove,
		int& moveCount,
		NextmoveEvent& mp,
		const bool cutNode
		)const override {
		// スプリット・ポイントはスルー☆！（＾ｑ＾）
	}

	virtual inline bool GetReturnBeforeStep20() const override {
		// スプリット・ポイントは　ステップ２０を実行する前に途中抜けするぜ☆（＾ｑ＾）
		return true;
	}

	inline void DoStep20(
		int& moveCount,
		Move& excludedMove,
		Rucksack& rucksack,
		ScoreIndex& alpha,
		Flashlight** ppFlashlight,//サーチスタック
		ScoreIndex& bestScore,
		int& playedMoveCount,
		ScoreIndex& beta,
		Key& posKey,
		const Depth depth,
		Move& bestMove,
		bool& inCheck,
		Position& pos,
		Move movesSearched[64]
		)const override {
		// スプリット・ポイントはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	inline Bound GetBoundAtStep20(bool bestMoveExists) const override {
		return Bound::BoundUpper;
	}

};


extern const NodetypeSplitedNodeNonPv g_NODETYPE_SPLITEDNODE_NON_PV;

