#pragma once

#include "../n110_square__/n110_500_utilSquare.hpp"


class ConfigBits {
public:
	ConfigBits();

	// 指定した位置の属する file の bit を shift し、
	// index を求める為に使用する。
	static const int m_slide[];//SquareNum
};



extern const int g_rookBlockBits[SquareNum];
extern const int g_bishopBlockBits[SquareNum];
extern const int g_rookShiftBits[SquareNum];
extern const int g_bishopShiftBits[SquareNum];

#if defined HAVE_BMI2
#else
extern const u64 g_rookMagic[SquareNum];
extern const u64 g_bishopMagic[SquareNum];
#endif




extern int		g_rookAttackIndex[SquareNum];


