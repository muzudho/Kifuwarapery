#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


// 位置関係、方向
// ボナンザそのまま
// でもあまり使わないので普通の enum と同様に 0 から順に値を付けて行けば良いと思う。
enum Direction {
	DirecMisc = Binary<  0>::value, // 縦、横、斜めの位置に無い場合
	DirecFile = Binary< 10>::value, // 縦
	DirecRank = Binary< 11>::value, // 横
	DirecDiagNESW = Binary<100>::value, // 右上から左下
	DirecDiagNWSE = Binary<101>::value, // 左上から右下
	DirecCross = Binary< 10>::value, // 縦、横
	DirecDiag = Binary<100>::value, // 斜め
};
OverloadEnumOperators(Direction);
