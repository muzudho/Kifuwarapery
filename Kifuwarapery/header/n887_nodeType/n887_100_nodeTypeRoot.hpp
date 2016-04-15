#pragma once


#include "../n119_score___/n119_090_score.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_500_searchStack.hpp"
#include "../n640_searcher/n640_440_splitPoint.hpp"
#include "../n883_nodeType/n883_500_nodeTypeAbstract.hpp"

#include "../n885_searcher/n885_500_searcher.hpp"
//class Searcher;


class NodeTypeRoot : public NodeTypeAbstract {
public:

	static void GoSearch(Searcher& searcher, Position& pos, SearchStack* ss, SplitPoint& sp) {

		/*
		Position a = pos;
		SearchStack* b = ss + 1;
		Score c = sp.m_alpha;
		Score d = sp.m_beta;
		Depth e = sp.m_depth;
		bool f = sp.m_cutNode;
		NodeType g = NodeType::SplitPointRoot;
		Searcher* h = pSearcher;
		 */

		searcher.Search<NodeType::SplitPointRoot >(pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

};


extern NodeTypeRoot g_nodeTypeRoot;

