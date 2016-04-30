#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


enum Color {
	Black,
	White,
	Null // 黒でも白でもないとき、例えば使っていない引数の穴を埋めるといったときに使う☆（＾ｑ＾）
};
OverloadEnumOperators(Color);
static const int g_COLOR_NUM = 3;

