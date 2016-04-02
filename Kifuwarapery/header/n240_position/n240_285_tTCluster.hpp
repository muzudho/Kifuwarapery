#pragma once

#include "../../header/n240_position/n240_280_tTEntry.hpp"

const int ClusterSize = CacheLineSize / sizeof(TTEntry);
static_assert(0 < ClusterSize, "");

struct TTCluster {
	TTEntry data[ClusterSize];
};
