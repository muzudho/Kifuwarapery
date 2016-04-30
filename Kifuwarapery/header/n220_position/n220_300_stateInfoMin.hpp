#pragma once

#include "../n105_color___/n105_500_convColor.hpp"
#include "../n119_score___/n119_100_utilScore.hpp"
#include "../n220_position/n220_250_changedLists.hpp"

// minimul stateinfo
struct StateInfoMin {
	ScoreIndex m_material; // stocfish の npMaterial は 先手、後手の点数を配列で持っているけど、
					// 特に分ける必要は無い気がする。
	int m_pliesFromNull;
	int m_continuousCheck[g_COLOR_NUM]; // Stockfish には無い。
};

