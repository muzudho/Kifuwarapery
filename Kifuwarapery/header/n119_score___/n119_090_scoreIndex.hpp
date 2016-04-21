#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n080_common__/n080_150_overloadEnumOperators.hpp"
#include "n119_055_utilBound.hpp"

using Ply = int;

const Ply g_maxPly = 128;
const Ply g_maxPlyPlus2 = g_maxPly + 2;

// 評価値
enum ScoreIndex {
	ScoreZero = 0,
	ScoreDraw = 0,
	ScoreMaxEvaluate = 30000,
	ScoreMateLong = 30002,
	ScoreMate1Ply = 32599,
	ScoreMate0Ply = 32600,
	ScoreMateInMaxPly = ScoreMate0Ply - g_maxPly,
	ScoreMatedInMaxPly = -ScoreMateInMaxPly,
	ScoreInfinite = 32601,
	ScoreNotEvaluated = INT_MAX,
	ScoreNone = 32602
};
OverloadEnumOperators(ScoreIndex);
