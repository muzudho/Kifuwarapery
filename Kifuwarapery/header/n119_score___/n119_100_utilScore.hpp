#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n080_common__/n080_150_overloadEnumOperators.hpp"
#include "n119_090_score.hpp"

using Ply = int;

class UtilScore {
public:
	static inline Score MateIn(const Ply ply) {
		return ScoreMate0Ply - static_cast<Score>(ply);
	}
	static inline Score MatedIn(const Ply ply) {
		return -ScoreMate0Ply + static_cast<Score>(ply);
	}

};


