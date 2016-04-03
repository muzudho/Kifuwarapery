#pragma once

#include "n200_050_bound.hpp"


class UtilBound {
public:
	// 使ってない？
	static inline bool ExactOrLower(const Bound st) {
		return (st & BoundLower ? true : false);
	}

	// 使ってない？
	static inline bool ExactOrUpper(const Bound st) {
		return (st & BoundUpper ? true : false);
	}
};
