#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n223_move____/n223_500_move.hpp"

enum Depth {
	OnePly = 2,
	Depth0 = 0,
	Depth1 = 1,
	DepthQChecks = -1 * OnePly,
	DepthQNoChecks = -2 * OnePly,
	DepthQRecaptures = -8 * OnePly,
	DepthNone = -127 * OnePly
};
OverloadEnumOperators(Depth);
