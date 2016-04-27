#include "../../header/n755_sword___/n755_100_SwordRoot.hpp"
#include "../../header/n887_nodeType/n887_130_nodetypeSplitedNodeRoot.hpp"


extern const NodetypeSplitedNodeRoot g_NODETYPE_SPLITEDNODE_ROOT;


const SwordRoot g_SWORD_ROOT;


// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
void SwordRoot::GoSearch_AsSplitedNode(
	SplitedNode& ownerSplitedNode,
	Rucksack& rucksack,
	Position& pos,
	Flashlight* pFlashlight
	) const {

	//────────────────────────────────────────────────────────────────────────────────
	// 探索☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// スプリットポイントは検索を開始するぜ☆（＾ｑ＾）
	g_NODETYPE_SPLITEDNODE_ROOT.GoToTheAdventure_new(
		rucksack,
		pos,
		pFlashlight + 1,
		ownerSplitedNode.m_alpha,
		ownerSplitedNode.m_beta,
		ownerSplitedNode.m_depth,
		ownerSplitedNode.m_cutNode
		);
}
