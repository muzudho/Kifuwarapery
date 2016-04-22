#pragma once


#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


enum Piece {
	// B* に 16 を加算することで、W* を表す。
	// Promoted を加算することで、成りを表す。
	N00_Empty = 0,
	UnPromoted = 0,
	Promoted = 8,
	N01_BPawn = 1,
	N02_BLance,
	N03_BKnight,
	N04_BSilver,
	N05_BBishop,
	N06_BRook,
	N07_BGold,
	N08_BKing,
	N09_BProPawn,
	N10_BProLance,
	N11_BProKnight,
	N12_BProSilver,
	N13_BHorse,
	N14_BDragon,
	// BDragon = 14
	N17_WPawn = 17,
	N18_WLance,
	N19_WKnight,
	N20_WSilver,
	N21_WBishop,
	N22_WRook,
	N23_WGold,
	N24_WKing,
	N25_WProPawn,
	N26_WProLance,
	N27_WProKnight,
	N28_WProSilver,
	N29_WHorse,
	N30_WDragon,
	N31_PieceNone // PieceNone = 31  これを 32 にした方が多重配列のときに有利か。
};
OverloadEnumOperators(Piece);
