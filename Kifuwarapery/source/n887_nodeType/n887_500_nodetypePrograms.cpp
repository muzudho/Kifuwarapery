#include "../../header/n883_nodeType/n883_070_nodetypeAbstract.hpp"
#include "../../header/n887_nodeType/n887_100_nodetypeRoot.hpp"
#include "../../header/n887_nodeType/n887_110_nodetypePv.hpp"
#include "../../header/n887_nodeType/n887_120_nodetypeNonPv.hpp"
#include "../../header/n887_nodeType/n887_130_nodetypeSplitedNodeRoot.hpp"
#include "../../header/n887_nodeType/n887_140_nodetypeSplitedNodePv.hpp"
#include "../../header/n887_nodeType/n887_150_nodetypeSplitedNodeNonPv.hpp"
#include "../../header/n887_nodeType/n887_500_nodetypePrograms.hpp"


NodetypeAbstract* g_NODETYPE_PROGRAMS[6] = {
	&g_nodetypeRoot,
	&g_nodetypePv,
	&g_nodetypeNonPv,
	&g_nodetypeSplitedNodeRoot,
	&g_nodetypeSplitedNodePv,
	&g_nodetypeSplitedNodeNonPv
};
