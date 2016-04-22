#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_500_rucksack.hpp"


class NodetypeSplitedNodePv : public NodetypeAbstract {
public:

	void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		UNREACHABLE;
	}

	const bool IsPvNode() const { return true; };
	const bool IsSplitedNode() const { return true; };
	const bool IsRootNode() const { return false; }

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
		) {
		// スプリット・ポイントにステップ１９は無いぜ☆（＾ｑ＾）
		UNREACHABLE;
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
		// スプリット・ポイントにステップ２０は無いぜ☆（＾ｑ＾）
		UNREACHABLE;
	}

};


extern NodetypeSplitedNodePv g_nodetypeSplitedNodePv;

