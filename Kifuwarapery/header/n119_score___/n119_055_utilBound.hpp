#pragma once

#include "n119_050_bound.hpp"


class UtilBound {
public:
	// �g���ĂȂ��H
	static inline bool ExactOrLower(const Bound st) {
		return (st & BoundLower ? true : false);
	}

	// �g���ĂȂ��H
	static inline bool ExactOrUpper(const Bound st) {
		return (st & BoundUpper ? true : false);
	}
};
