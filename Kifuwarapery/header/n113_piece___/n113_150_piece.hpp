#pragma once

#include "../n113_piece___/n113_105_utilPieceType.hpp"



enum Piece {
	// B* に 16 を加算することで、W* を表す。
	// Promoted を加算することで、成りを表す。
	Empty = 0, UnPromoted = 0, Promoted = 8,
	BPawn = 1, BLance, BKnight, BSilver, BBishop, BRook, BGold, BKing,
	BProPawn, BProLance, BProKnight, BProSilver, BHorse, BDragon, // BDragon = 14
	WPawn = 17, WLance, WKnight, WSilver, WBishop, WRook, WGold, WKing,
	WProPawn, WProLance, WProKnight, WProSilver, WHorse, WDragon,
	PieceNone // PieceNone = 31  これを 32 にした方が多重配列のときに有利か。
};
OverloadEnumOperators(Piece);
