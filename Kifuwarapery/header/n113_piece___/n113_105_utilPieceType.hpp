#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_100_pieceType.hpp"



// pc が遠隔駒であるかを調べるのに使います。
const u32 g_isSliderVal = 0x60646064;

class UtilPieceType {
public:

	// pc が遠隔駒であるか
	static inline bool IsSlider(const PieceType pt) { return (g_isSliderVal & (1 << pt)) != 0; }

};

