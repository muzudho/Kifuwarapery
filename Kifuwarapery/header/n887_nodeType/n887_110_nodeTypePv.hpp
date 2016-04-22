﻿#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../n885_searcher/n885_500_rucksack.hpp"


class NodetypePv : public NodetypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	void GoSearch(Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		Hitchhiker::Travel(	rucksack, NodeType::SplitedNodePV,
			pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

	const bool IsPvNode() const { return true; };
	const bool IsSplitedNode() const { return false; };
	const bool IsRootNode() const { return false; }

};


extern NodetypePv g_nodetypePv;

