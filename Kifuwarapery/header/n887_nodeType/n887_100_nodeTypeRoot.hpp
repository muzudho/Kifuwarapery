#pragma once


#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


class NodetypeRoot : public NodetypeAbstract {
public:

	void GoSearch(Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		//────────────────────────────────────────────────────────────────────────────────
		// 探索☆？（＾ｑ＾）
		//────────────────────────────────────────────────────────────────────────────────
		Hitchhiker::Travel_885_510(	rucksack, NodeType::SplitedNodeRoot,
			pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

	const bool IsPvNode() const { return true; };
	const bool IsSplitedNode() const { return false; };
	const bool IsRootNode() const { return true; }

	inline Bound GetBoundAtStep20(bool bestMoveExists) const {
		return bestMoveExists ? Bound::BoundExact : Bound::BoundUpper;
	}

};


extern NodetypeRoot g_nodetypeRoot;

