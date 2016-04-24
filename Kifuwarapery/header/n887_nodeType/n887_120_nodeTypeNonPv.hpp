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
	inline void GoSearch(Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp) const {
		//────────────────────────────────────────────────────────────────────────────────
		// 探索☆？（＾ｑ＾）
		//────────────────────────────────────────────────────────────────────────────────
		Hitchhiker::Travel_885_510(	rucksack, NodeType::SplitedNodeNonPV,
			pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
	}

	inline const bool IsPvNode() const { return false; };
	inline const bool IsSplitedNode() const { return false; };
	inline const bool IsRootNode() const { return false; }

	inline void DoStep1c(
		Military** ppThisThread,
		Flashlight** ppFlashlight
		) {
		// PVノードではないので空っぽだぜ☆（＾ｑ＾）
	}

	// ルートノードか、それ以外かで　値が分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep4x(
		Move& ttMove,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		Position& pos
		)
	{
		ttMove = pTtEntry != nullptr ?
			UtilMoveStack::Move16toMove(pTtEntry->GetMove(), pos) :
			g_MOVE_NONE;
	}

	inline Bound GetBoundAtStep20(bool bestMoveExists) const {
		return Bound::BoundUpper;
	}

};


extern NodetypeNonPv g_nodetypeNonPv;

