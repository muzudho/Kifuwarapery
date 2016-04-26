#include "../../header/n887_nodeType/n887_110_nodetypePv.hpp"
#include "../../header/n887_nodeType/n887_140_nodetypeSplitedNodePv.hpp"


extern const NodetypeSplitedNodePv g_NODETYPE_SPLITEDNODE_PV;


const NodetypePv g_NODETYPE_PV;


// 依存関係の都合上、インラインにはしないぜ☆（＾ｑ＾）
void NodetypePv::GoSearch_AsSplitedNode(
	Rucksack& rucksack, Position& pos, Flashlight* ss, SplitedNode& sp
	) const {
	//────────────────────────────────────────────────────────────────────────────────
	// 探索☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// スプリットポイントに変えて探索かだぜ☆（＾ｑ＾）
	g_NODETYPE_SPLITEDNODE_PV.
		GoToTheAdventure_new(rucksack, pos, ss + 1, sp.m_alpha, sp.m_beta, sp.m_depth, sp.m_cutNode);
}
