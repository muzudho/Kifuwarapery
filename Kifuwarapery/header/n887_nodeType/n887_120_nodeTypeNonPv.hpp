#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


class NodetypeNonPv : public NodetypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	void GoSearch(Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		//────────────────────────────────────────────────────────────────────────────────
		// 探索☆？（＾ｑ＾）
		//────────────────────────────────────────────────────────────────────────────────
		Hitchhiker::Travel_885_510(	rucksack, NodeType::SplitedNodeNonPV,
			pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

	const bool IsPvNode() const { return false; };
	const bool IsSplitedNode() const { return false; };
	const bool IsRootNode() const { return false; }

	inline Bound GetBoundAtStep20(bool bestMoveExists) const {
		return Bound::BoundUpper;
	}

};


extern NodetypeNonPv g_nodetypeNonPv;

