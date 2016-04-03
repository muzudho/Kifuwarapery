#pragma once

#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n200_score___/n200_100_utilScore.hpp"
#include "../n220_position/n220_250_changedLists.hpp"

// minimul stateinfo
struct StateInfoMin {
	Score material; // stocfish の npMaterial は 先手、後手の点数を配列で持っているけど、
					// 特に分ける必要は無い気がする。
	int pliesFromNull;
	int continuousCheck[ColorNum]; // Stockfish には無い。
};

