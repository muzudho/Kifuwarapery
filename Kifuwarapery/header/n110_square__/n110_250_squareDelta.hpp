#pragma once

#include "../n110_square__/n110_205_utilRank.hpp"

enum SquareDelta {
	DeltaNothing = 0, // 同一の Square にあるとき
	DeltaN = -1, DeltaE = -9, DeltaS = 1, DeltaW = 9,
	DeltaNE = DeltaN + DeltaE,
	DeltaSE = DeltaS + DeltaE,
	DeltaSW = DeltaS + DeltaW,
	DeltaNW = DeltaN + DeltaW
};
OverloadEnumOperators(SquareDelta);
