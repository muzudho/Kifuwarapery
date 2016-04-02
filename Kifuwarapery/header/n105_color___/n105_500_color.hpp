#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"

enum Color {
	Black, White, ColorNum
};
OverloadEnumOperators(Color);

// 相手の色
inline constexpr Color OppositeColor(const Color c) {
	return static_cast<Color>(static_cast<int>(c) ^ 1);
}

