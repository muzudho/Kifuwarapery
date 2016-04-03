#pragma once

#include "../n113_piece___/n113_105_utilPieceType.hpp"



enum Piece {
	// B* �� 16 �����Z���邱�ƂŁAW* ��\���B
	// Promoted �����Z���邱�ƂŁA�����\���B
	Empty = 0, UnPromoted = 0, Promoted = 8,
	BPawn = 1, BLance, BKnight, BSilver, BBishop, BRook, BGold, BKing,
	BProPawn, BProLance, BProKnight, BProSilver, BHorse, BDragon, // BDragon = 14
	WPawn = 17, WLance, WKnight, WSilver, WBishop, WRook, WGold, WKing,
	WProPawn, WProLance, WProKnight, WProSilver, WHorse, WDragon,
	PieceNone // PieceNone = 31  ����� 32 �ɂ����������d�z��̂Ƃ��ɗL�����B
};
OverloadEnumOperators(Piece);
