#pragma once

#include "../n240_tt______/n240_280_tTEntry.hpp"

const int g_ClusterSize = CacheLineSize / sizeof(TTEntry);
static_assert(0 < g_ClusterSize, "");

struct TTCluster {
	TTEntry m_data[g_ClusterSize];
};
