#pragma once

#include "../n110_square__/n110_155_utilFile.hpp"

enum Rank {
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1, RankNum
};
OverloadEnumOperators(Rank);



const Rank SquareToRank[SquareNum] = {
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1,
	Rank9, Rank8, Rank7, Rank6, Rank5, Rank4, Rank3, Rank2, Rank1
};
