#pragma once

#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_500_utilSquare.hpp"

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