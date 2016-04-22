#include "../../header/n883_nodeType/n883_500_nodeTypeAbstract.hpp"
#include "../../header/n887_nodeType/n887_100_nodeTypeRoot.hpp"
#include "../../header/n887_nodeType/n887_110_nodeTypePv.hpp"
#include "../../header/n887_nodeType/n887_120_nodeTypeNonPv.hpp"
#include "../../header/n887_nodeType/n887_500_nodeTypeArray.hpp"


NodeTypeAbstract* g_nodeTypeArray[3] = {
	&g_nodeTypeRoot,
	&g_nodeTypePv,
	&g_nodeTypeNonPv
};
