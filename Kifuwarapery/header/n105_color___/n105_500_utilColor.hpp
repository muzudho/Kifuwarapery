#pragma once

#include "n105_100_color.hpp"


class UtilColor {
public:
	// 相手の色
	static inline constexpr Color OppositeColor(const Color c) {
		return static_cast<Color>(static_cast<int>(c) ^ 1);
	}
};


