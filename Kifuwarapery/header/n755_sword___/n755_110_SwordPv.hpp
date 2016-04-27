#pragma once


#include "n755_070_SwordAbstract.hpp"


class SwordPv : public SwordAbstract {
public:

	// スプリット・ポイントが検索するぜ☆（＾ｑ＾）
	// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
	virtual void GoSearch_AsSplitedNode(
		SplitedNode& ownerSplitedNode,
		Rucksack& searcher,
		Position& pos,
		Flashlight* pFlashlight
		) const override;

};


extern const SwordPv g_SWORD_PV;
