#pragma once

#include "../n113_piece___/n113_100_pieceType.hpp"



// pc �����u��ł��邩�𒲂ׂ�̂Ɏg���܂��B
const u32 g_isSliderVal = 0x60646064;

class UtilPieceType {
public:

	// pc �����u��ł��邩
	static inline bool IsSlider(const PieceType pt) { return (g_isSliderVal & (1 << pt)) != 0; }

};

