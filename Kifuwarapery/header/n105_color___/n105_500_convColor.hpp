#pragma once

#include "n105_100_color.hpp"


class ConvColor {
public:

	// 相手の色
	template<Color CLR>
	static inline constexpr Color OPPOSITE_COLOR10() {
		return static_cast<Color>(static_cast<int>(CLR) ^ 1);
	}
	static inline constexpr Color OPPOSITE_COLOR10b(const Color c) {
		return static_cast<Color>(static_cast<int>(c) ^ 1);
	}

};


