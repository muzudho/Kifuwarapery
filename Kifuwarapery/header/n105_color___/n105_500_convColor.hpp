#pragma once

#include "n105_100_color.hpp"


class ConvColor {
public:
	// 相手の色
	static inline constexpr Color OPPOSITE_COLOR10(const Color c) {
		return static_cast<Color>(static_cast<int>(c) ^ 1);
	}
};


