#pragma once

#include "../../stdafx.h"
#include "../n080_common__/n080_150_overloadEnumOperators.hpp"

enum Color {
	Black, White, ColorNum
};
OverloadEnumOperators(Color);

inline constexpr Color oppositeColor(const Color c) {
	return static_cast<Color>(static_cast<int>(c) ^ 1);
}

