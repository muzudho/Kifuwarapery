#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n883_nodeType/n883_500_nodeTypeAbstract.hpp"
#include "../n885_searcher/n885_500_searcher.hpp"


class NodeTypePv : public NodeTypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	static void GoSearch(Searcher& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) {
		searcher.Search<NodeType::SplitedNodePV   >(pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

};


extern NodeTypePv g_nodeTypePv;

