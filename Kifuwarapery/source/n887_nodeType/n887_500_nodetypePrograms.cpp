#include "../../header/n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../../header/n887_nodeType/n887_100_nodetypeRoot.hpp"
#include "../../header/n887_nodeType/n887_110_nodetypePv.hpp"
#include "../../header/n887_nodeType/n887_120_nodetypeNonPv.hpp"
#include "../../header/n887_nodeType/n887_130_nodetypeSplitedNodeRoot.hpp"
#include "../../header/n887_nodeType/n887_140_nodetypeSplitedNodePv.hpp"
#include "../../header/n887_nodeType/n887_150_nodetypeSplitedNodeNonPv.hpp"
#include "../../header/n887_nodeType/n887_500_nodetypePrograms.hpp"


// サーチ☆
// 主に、反復深化探索の中で呼び出される☆
const NodetypeAbstract* g_NODETYPE_PROGRAMS[6] = {
	&g_NODETYPE_ROOT,
	&g_NODETYPE_PV,
	&g_NODETYPE_NON_PV,
	&g_NODETYPE_SPLITEDNODE_ROOT,
	&g_NODETYPE_SPLITEDNODE_PV,
	&g_NODETYPE_SPLITEDNODE_NON_PV
};
