#pragma once

#include "../../stdafx.h"
#include "../n120_brdEntry/n120_100_color.hpp"
#include "../n120_brdEntry/n120_150_square.hpp"

class ColorFileRank {
public:
	Color color;
	File file;
	Rank rank;
public:
	ColorFileRank(Color color, File file, Rank rank);

	// ソート用の数字。
	int GetOrder();
};