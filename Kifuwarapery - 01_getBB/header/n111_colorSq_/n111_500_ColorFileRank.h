#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_150_file.hpp"
#include "../n110_square__/n110_200_rank.hpp"

class ColorFileRank {
public:

	Color m_color;

	File m_file;

	Rank m_rank;

public:

	ColorFileRank(Color color, File file, Rank rank);

	// ソート用の数字。
	int GetOrder();

};