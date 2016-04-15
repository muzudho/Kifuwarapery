#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_500_searchStack.hpp"
#include "../n640_searcher/n640_440_splitPoint.hpp"

//#include "../n885_searcher/n885_500_searcher.hpp" //FIXME:
//class Searcher;


class NodeTypeAbstract {
public:

	//virtual
	static void GoSearch(Searcher& searcher, Position& pos, SearchStack* ss, SplitPoint& sp) {};

};


