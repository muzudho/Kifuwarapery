#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"
#include "../n110_square__/n110_100_square.hpp"

enum Rank {
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1, RankNum
};
OverloadEnumOperators(Rank);


extern const Rank g_squareToRank[SquareNum];
