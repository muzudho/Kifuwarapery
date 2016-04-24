#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


class NodetypeSplitedNodeNonPv : public NodetypeAbstract {
public:

	inline void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		UNREACHABLE;
	}

	inline const bool IsPvNode() const { return false; };
	inline const bool IsSplitedNode() const { return true; };
	inline const bool IsRootNode() const { return false; }

	inline void DoStep1c(
		Military** ppThisThread,
		Flashlight** ppFlashlight
		) {
		// PVノードではないので空っぽだぜ☆（＾ｑ＾）
	}

	// ルートノードか、それ以外かで　値が分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep4x(
		Move& ttMove,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		Position& pos
		)
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
		)
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
		) {
		// 非ルートノードには無いぜ☆！（＾ｑ＾）
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
		) {
		// スプリット・ポイントにステップ１９は無いぜ☆（＾ｑ＾）
		UNREACHABLE;
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
		) {
		// スプリット・ポイントにステップ２０は無いぜ☆（＾ｑ＾）
		UNREACHABLE;
	}

	inline Bound GetBoundAtStep20(bool bestMoveExists) const {
		return Bound::BoundUpper;
	}

};


extern NodetypeSplitedNodeNonPv g_nodetypeSplitedNodeNonPv;

