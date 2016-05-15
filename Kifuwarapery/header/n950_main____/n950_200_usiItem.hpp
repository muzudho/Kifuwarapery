#pragma once


#include "../n220_position/n220_650_position.hpp"


class UsiItem{
public:
#if !defined MINIMUL
	// for debug
	// 指し手生成の速度を計測
	static void MeasureGenerateMoves(const Position& pos);
#endif
};

