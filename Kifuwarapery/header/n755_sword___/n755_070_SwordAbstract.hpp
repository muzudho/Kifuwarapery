#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp" // Rucksackと持ち合い
class Rucksack;


// スプリット・ポイントは、剣を持っているんだぜ☆（＾ｑ＾）
class SwordAbstract {
public:

	// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
	virtual void GoSearch_AsSplitedNode(
		SplitedNode& ownerSplitedNode,
		Rucksack& searcher,
		Position& pos,
		Flashlight* pFlashlight
		) const = 0;

};
