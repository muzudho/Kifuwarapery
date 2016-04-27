#include "../../header/n755_sword___/n755_120_SwordNonPv.hpp"
#include "../../header/n887_nodeType/n887_150_nodetypeSplitedNodeNonPv.hpp"


extern const NodetypeSplitedNodeNonPv g_NODETYPE_SPLITEDNODE_NON_PV;


const SwordNonPv g_SWORD_NON_PV;


// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
void SwordNonPv::GoSearch_AsSplitedNode(
	SplitedNode& ownerSplitedNode, 
	Rucksack& rucksack,
	Position& pos,
	Flashlight* pFlashlight
	) const {
	//────────────────────────────────────────────────────────────────────────────────
	// 探索☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// スプリットポイントは検索を開始するぜ☆（＾ｑ＾）
	g_NODETYPE_SPLITEDNODE_NON_PV.GoToTheAdventure_new(
		rucksack,
		pos,
		pFlashlight + 1,
		ownerSplitedNode.m_alpha,
		ownerSplitedNode.m_beta,
		ownerSplitedNode.m_depth,
		ownerSplitedNode.m_cutNode
		);
}
