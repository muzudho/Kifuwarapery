#pragma once

#include "../n110_square__/n110_500_utilSquare.hpp"


class ConfigBits {
public:
	ConfigBits();

	// 指定した位置の属する file の bit を shift し、
	// index を求める為に使用する。
	static const int m_slide[SquareNum];

	static const int m_rookBlockBits[SquareNum];

	static const int m_bishopBlockBits[SquareNum];
	static const int m_rookShiftBits[SquareNum];
	static const int m_bishopShiftBits[SquareNum];

	#if defined HAVE_BMI2
	#else
		static const u64 m_rookMagic[SquareNum];
		static const u64 m_bishopMagic[SquareNum];
	#endif

	static int		m_rookAttackIndex[SquareNum];

};
