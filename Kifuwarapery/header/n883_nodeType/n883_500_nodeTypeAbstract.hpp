#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp" // Searcherと持ち合い
class Rucksack;


class NodeTypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	static void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) {};

};


