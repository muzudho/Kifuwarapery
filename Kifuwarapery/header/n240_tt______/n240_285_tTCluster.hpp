#pragma once

#include "../n240_tt______/n240_280_tTEntry.hpp"


const int g_clusterSize = CacheLineSize / sizeof(TTEntry);
static_assert(0 < g_clusterSize, "");


struct TTCluster {
	TTEntry m_data[g_clusterSize];
};
