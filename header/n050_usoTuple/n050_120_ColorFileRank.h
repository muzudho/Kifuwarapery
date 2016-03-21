#pragma once

#include "../../stdafx.h"
#include "../../color.hpp"
#include "../../square.hpp"

class ColorFileRank {
public:
	Color color;
	File file;
	Rank rank;
public:
	ColorFileRank(Color color, File file, Rank rank);

	// �\�[�g�p�̐����B
	int GetOrder();
};