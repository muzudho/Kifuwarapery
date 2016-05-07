#pragma once


#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n640_searcher/n640_500_reductions.hpp"
#include "../n755_sword___/n755_100_SwordRoot.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"
#include "../n887_nodeType/n887_100_nodetypeRoot.hpp"


// PvNode = true
// SplitedNode = false
// IsRootNode = true
class NodetypeRoot : public NodetypeAbstract {
public:

	//*
	virtual ScoreIndex GoToTheAdventure_new(
		Rucksack& rucksack,
		Position& pos,
		Flashlight* pFlashlight,//サーチスタック
		ScoreIndex alpha,
		ScoreIndex beta,
		const Depth depth,
		const bool cutNode
		) const override;
	//*/

	// 非PVノードはassertをするぜ☆（＾ｑ＾）
	virtual inline void AssertBeforeStep1(
		ScoreIndex alpha,
		ScoreIndex beta
		) const override {
		// PVノードはスルー☆！（＾ｑ＾）
		assert(alpha == beta - 1);
	}

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
		)const override {
		// 非スプリット・ポイントはスルー☆！（＾ｑ＾）
	}

	virtual inline void DoStep2(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Position& pos,
		Rucksack& rucksack,
		Flashlight** ppFlashlight
		)const override
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
		)const override {
		// ルートには無いぜ☆（＾ｑ＾）！
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
		)const override {
		// ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// PVノードの場合☆（＾ｑ＾）
	virtual inline bool GetConditionInStep4y(
		const TTEntry* pTtEntry,
		ScoreIndex& beta,
		ScoreIndex& ttScore
		) const override {
		return pTtEntry->GetType() == Bound::BoundExact;
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
		)const override {
		// ルートノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep6_NonPV(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		const Depth depth,
		ScoreIndex& eval,
		ScoreIndex& beta,
		Move& ttMove,
		Position& pos,
		Flashlight** ppFlashlight
		)const override {
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
		)const override {
		// PVノードはスルー☆！（＾ｑ＾）
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep8_NonPV(
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
		)const override {
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
		)const override {
		// 非PVノードはスルー☆！（＾ｑ＾）
	}

	// PVノードか、そうでないかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void DoStep10_InternalIterativeDeepening(
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
			// PVノードの場合、５倍☆
			(5 * OnePly) <= depth // 深さが５以上で☆
			&&
			ttMove.IsNone()	// トランスポジション・テーブルに指し手がない場合☆
			)
		{
			//const Depth d = depth - 2 * OnePly - (PVNode ? Depth0 : depth / 4);
			// PVノードの場合☆
			const Depth d = depth - 2 * OnePly; // 2手戻した深さ☆

			(*ppFlashlight)->m_skipNullMove = true;

			//────────────────────────────────────────────────────────────────────────────────
			// 探索☆？（＾ｑ＾）
			//────────────────────────────────────────────────────────────────────────────────
			// PVノードの場合☆
			g_NODETYPE_PROGRAMS[NodeType::N01_PV]->GoToTheAdventure_new(
				rucksack, pos, (*ppFlashlight), alpha, beta, d, true);

			(*ppFlashlight)->m_skipNullMove = false;

			(*ppTtEntry) = rucksack.m_tt.Probe(posKey);

			ttMove = (
				(*ppTtEntry) != nullptr
				?
				UtilMove01::Move16toMove((*ppTtEntry)->GetMove(), pos)
				:
				g_MOVE_NONE
				);
		}
	}

	virtual inline ScoreIndex GetBetaAtStep11(
		ScoreIndex beta
		) const override {
		// PVノードの場合☆（＾ｑ＾）
		return -ScoreIndex::ScoreInfinite;
	}

	virtual inline Move GetNextMove_AtStep11(
		NextmoveEvent& mp
		) const override {
		// 非スプリットポイントの場合
		return mp.GetNextMove_NonSplitedNode();
	};

	virtual inline void DoStep11a_BeforeLoop_SplitPointStart(
		Move& ttMove,
		const Depth depth,
		ScoreIndex& score,
		ScoreIndex& bestScore,
		bool& singularExtensionNode,
		Move& excludedMove,
		const TTEntry* pTtEntry
		)const override
	{
		// ルートはこういう感じ☆（＾ｑ＾）
		score = bestScore;
		singularExtensionNode = false;
	}

	// スプリット・ポイントかどうかで変わる手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep11c_LoopHeader(
		bool& isContinue,
		Position& pos,
		Move& move,
		const CheckInfo& ci,
		int& moveCount,
		SplitedNode** ppSplitedNode
		) const override {
			++moveCount;
	}

	// 無駄枝狩り☆（＾▽＾）
	virtual inline void DoStep13a_FutilityPruning(
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
		) const override {
		// PVノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	// PVノードか、そうでないかで変わるぜ☆！（＾ｑ＾）
	virtual inline const Depth GetPredictedDepthInStep13a(
		Depth& newDepth,
		const Depth depth,
		int& moveCount
		) const override {
		// PVノードのとき
		return newDepth - g_reductions.DoReduction_PvNode(depth, moveCount);
	}

	virtual inline void LockInStep13a(
		SplitedNode** ppSplitedNode
		) const override
	{
		// 非スプリット・ポイントではスルー☆！（＾ｑ＾）
	}
	virtual inline void LockAndUpdateBestScoreInStep13a(
		SplitedNode** ppSplitedNode,
		ScoreIndex& bestScore
		) const override {
		// 非スプリット・ポイントではスルー☆！（＾ｑ＾）
	}

	virtual inline void DoStep13b(
		bool& isContinue,
		Position& pos,
		Move& move,
		const CheckInfo& ci,
		int& moveCount
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

		// PVノードだぜ☆！（＾ｑ＾）
		isPVMove = (moveCount == 1);
		(*ppFlashlight)->m_currentMove = move;
	}

	virtual inline void UpdateAlphaInStep15(
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode
		) const override {

		// 非スプリットノードではスルー☆！（＾ｑ＾）
	}

	// Pvノードかどうかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void SetReductionInStep15(
		Flashlight** ppFlashlight,
		const Depth depth,
		int& moveCount,
		const bool cutNode
		) const override {
		// Pvノードのとき☆！（＾ｑ＾）
		(*ppFlashlight)->m_reduction = g_reductions.DoReduction_PvNode(depth, moveCount);
	}

	virtual inline void DoStep16a(
		bool& doFullDepthSearch,
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode
		)const override {
		// 非スプリットノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline bool IsBetaLargeAtStep16c(
		ScoreIndex& score,
		ScoreIndex& beta
		) const override {
		// ルートノードの場合☆（＾ｑ＾）
		return true;
	}

	virtual inline void DoStep18a(
		SplitedNode** ppSplitedNode,
		ScoreIndex& bestScore,
		ScoreIndex& alpha
		)const override {
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
		)const override {

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

	// 非スプリットポイントでだけ実行するぜ☆（＾ｑ＾）
	virtual inline void DoStep19(
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

		if (
			rucksack.m_ownerHerosPub.GetMinSplitDepth() <= depth
			&&
			rucksack.m_ownerHerosPub.GetAvailableSlave(*ppThisThread)
			&&
			(*ppThisThread)->m_splitedNodesSize < g_MaxSplitedNodesPerThread)
		{
			assert(bestScore < beta);
			(*ppThisThread)->ForkNewFighter<Rucksack::FakeSplit>(
				pos,
				*ppFlashlight,
				alpha,
				beta,
				bestScore,
				bestMove,
				depth,
				threatMove,
				moveCount,
				mp,
				&g_SWORD_ROOT,
				cutNode
				);

			if (beta <= bestScore) {
				isBreak = true;
				return;
			}
		}

	}

	virtual inline bool GetReturnBeforeStep20() const override {
		// 非スプリット・ポイントは　ステップ２０を実行する前に途中抜けはしないぜ☆（＾ｑ＾）
		return false;
	}

	virtual inline Bound GetBoundAtStep20(bool bestMoveExists) const override {
		return bestMoveExists ? Bound::BoundExact : Bound::BoundUpper;
	}

};


extern NodetypeRoot g_NODETYPE_ROOT;

