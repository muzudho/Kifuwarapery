#pragma once

#include "../n113_piece___/n113_100_pieceType.hpp"



// pc ‚ª‰“Šu‹î‚Å‚ ‚é‚©‚ğ’²‚×‚é‚Ì‚Ég‚¢‚Ü‚·B
const u32 g_isSliderVal = 0x60646064;

class UtilPieceType {
public:

	// pc ‚ª‰“Šu‹î‚Å‚ ‚é‚©
	static inline bool IsSlider(const PieceType pt) { return (g_isSliderVal & (1 << pt)) != 0; }

};

