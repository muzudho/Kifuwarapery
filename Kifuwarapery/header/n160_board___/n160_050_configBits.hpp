#pragma once

#include "../n110_square__/n110_500_utilSquare.hpp"


class ConfigBits {
public:
	ConfigBits();

	// �w�肵���ʒu�̑����� file �� bit �� shift ���A
	// index �����߂�ׂɎg�p����B
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


