#pragma once


#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


class NodetypeSplitedNodeRoot : public NodetypeAbstract {
public:

	inline void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		UNREACHABLE;
	}

	inline const bool IsPvNode() const { return true; };
	inline const bool IsSplitedNode() const { return true; };
	inline const bool IsRootNode() const { return true; }

	virtual inline void DoStep3(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Flashlight** ppFlashlight,
		ScoreIndex& alpha,
		ScoreIndex& beta
		) {
		// ルートには無いぜ☆（＾ｑ＾）！
	}

	// ルートノードか、それ以外かで　値が分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep4x(
		Move& ttMove,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		Position& pos
		)
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
		) {
		// ルートノードには無いぜ☆（＾ｑ＾）
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
		) {
		// ルートノードには無いぜ☆（＾ｑ＾）
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
		// ルートはこういう感じ☆（＾ｑ＾）
		score = bestScore;
		singularExtensionNode = false;
	}

	virtual inline void DoStep19(
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
		return bestMoveExists ? Bound::BoundExact : Bound::BoundUpper;
	}

};


extern NodetypeSplitedNodeRoot g_nodetypeSplitedNodeRoot;

