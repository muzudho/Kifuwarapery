#include "../../header/n887_nodeType/n887_120_nodetypeNonPv.hpp"
#include "../../header/n887_nodeType/n887_150_nodetypeSplitedNodeNonPv.hpp"


extern const NodetypeSplitedNodeNonPv g_NODETYPE_SPLITEDNODE_NON_PV;


const NodetypeNonPv g_NODETYPE_NON_PV;


// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
void NodetypeNonPv::GoSearch_AsSplitedNode(
	Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp
	) const {
	//────────────────────────────────────────────────────────────────────────────────
	// 探索☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// スプリットポイントに変えて探索かだぜ☆（＾ｑ＾）
	g_NODETYPE_SPLITEDNODE_NON_PV.GoToTheAdventure_new(rucksack, pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
}
