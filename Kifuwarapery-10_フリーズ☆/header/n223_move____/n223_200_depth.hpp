#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


enum Depth {
	OnePly = 2,//先後で 1 Ply☆？（＾ｑ＾）？チェスの数え方なのかだぜ☆？
	Depth0 = 0,
	Depth1 = 1,
	DepthQChecks = -1 * OnePly,
	DepthQNoChecks = -2 * OnePly,
	DepthQRecaptures = -8 * OnePly,
	DepthNone = -127 * OnePly
};
OverloadEnumOperators(Depth);
