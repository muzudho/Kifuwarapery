#pragma once

#include "../n105_color___/n105_500_color.hpp"
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