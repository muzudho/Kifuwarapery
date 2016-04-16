#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_500_searchStack.hpp"
#include "../n640_searcher/n640_440_splitPoint.hpp"
class Searcher;


class NodeTypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	static void GoSearch(Searcher& searcher, Position& pos, SearchStack* ss, SplitPoint& sp) {};

};


