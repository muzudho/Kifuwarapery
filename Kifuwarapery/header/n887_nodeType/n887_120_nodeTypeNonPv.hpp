#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


// RootNode = false;
class NodetypeNonPv : public NodetypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	inline void GoSearch(Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		//────────────────────────────────────────────────────────────────────────────────
		// 探索☆？（＾ｑ＾）
		//────────────────────────────────────────────────────────────────────────────────
		Hitchhiker::Travel_885_510(	rucksack, NodeType::SplitedNodeNonPV,
			pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

	inline const bool IsPvNode() const { return false; };
	inline const bool IsSplitedNode() const { return false; };

	inline void DoStep1c(
		Military** ppThisThread,
		Flashlight** ppFlashlight
		)const {
		// 非PVノードはスルー☆！（＾ｑ＾）
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
		ttMove = pTtEntry != nullptr ?
			UtilMoveStack::Move16toMove(pTtEntry->GetMove(), pos) :
			g_MOVE_NONE;
	}

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
		singularExtensionNode = !this - IsSplitedNode()
			&& 8 * Depth::OnePly <= depth
			&& !ttMove.IsNone()
			&& excludedMove.IsNone()
			&& (pTtEntry->GetType() & BoundLower)
			&& depth - 3 * Depth::OnePly <= pTtEntry->GetDepth();
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
			++moveCount;
	}

	virtual inline void DoStep16a(
		bool& doFullDepthSearch,
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode
		)const {
		// 非スプリットノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
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
		)const {
		// 非PVノードはスルー☆！（＾ｑ＾）
		//UNREACHABLE;
	}

	virtual inline bool IsBetaLargeAtStep16c(
		ScoreIndex& score,
		ScoreIndex& beta
		) const {
		// 非ルートノードの場合☆（＾ｑ＾）
		return score < beta;
	}

	virtual inline void DoStep18a(
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

	// 非スプリット・ポイントの場合☆（＾ｑ＾）
	virtual inline void DoStep18b(
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

				if (this->IsPvNode() && score < beta) {
					alpha = score;
				}
				else {
					isBreak = true;
					return;
				}
			}
		}

	}

	inline Bound GetBoundAtStep20(bool bestMoveExists) const {
		return Bound::BoundUpper;
	}

};


extern NodetypeNonPv g_nodetypeNonPv;

