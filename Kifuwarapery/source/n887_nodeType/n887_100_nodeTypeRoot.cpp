#include "../../header/n887_nodeType/n887_100_nodetypeRoot.hpp"
#include "../../header/n887_nodeType/n887_130_nodetypeSplitedNodeRoot.hpp"


extern const NodetypeSplitedNodeRoot g_NODETYPE_SPLITEDNODE_ROOT;


const NodetypeRoot g_NODETYPE_ROOT;


// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
void NodetypeRoot::GoSearch_AsSplitedNode(
	Rucksack& rucksack,
	Position& pos,
	Flashlight* ss,
	SplitedNode& sp
	) const {

	//────────────────────────────────────────────────────────────────────────────────
	// 探索☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// スプリットポイントに変えて探索かだぜ☆（＾ｑ＾）
	g_NODETYPE_SPLITEDNODE_ROOT.GoToTheAdventure_new(
		rucksack, pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
}
