#include "../../header/n755_sword___/n755_110_SwordPv.hpp"
#include "../../header/n887_nodeType/n887_140_nodetypeSplitedNodePv.hpp"


//extern const NodetypeSplitedNodePv g_NODETYPE_SPLITEDNODE_PV;


const SwordPv g_SWORD_PV;


// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
void SwordPv::GoSearch_AsSplitedNode(
	SplitedNode& ownerSplitedNode, 
	Rucksack& rucksack,
	Position& pos,
	Flashlight* pFlashlight
	) const {
	//────────────────────────────────────────────────────────────────────────────────
	// 探索☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// スプリットポイントは検索を開始するぜ☆（＾ｑ＾）
	g_NODETYPE_SPLITEDNODE_PV.GoToTheAdventure_new(
		rucksack,
		pos,
		pFlashlight + 1,
		ownerSplitedNode.m_alpha,
		ownerSplitedNode.m_beta,
		ownerSplitedNode.m_depth,
		ownerSplitedNode.m_cutNode
		);
}
