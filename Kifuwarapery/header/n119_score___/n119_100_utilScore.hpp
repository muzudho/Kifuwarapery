#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n080_common__/n080_150_overloadEnumOperators.hpp"
#include "n119_090_scoreIndex.hpp"

using Ply = int;

class UtilScore {
public:
	static inline ScoreIndex MateIn(const Ply ply) {
		return ScoreMate0Ply - static_cast<ScoreIndex>(ply);
	}
	static inline ScoreIndex MatedIn(const Ply ply) {
		return -ScoreMate0Ply + static_cast<ScoreIndex>(ply);
	}

};


