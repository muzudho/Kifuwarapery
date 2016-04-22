#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"


// 持ち駒を表すときに使用する。
// todo: HGold を HRook の後ろに持っていき、PieceType との変換を簡単に出来るようにする。
enum HandPiece {
	HPawn, HLance, HKnight, HSilver, HGold, HBishop, HRook, HandPieceNum
};
OverloadEnumOperators(HandPiece);


extern const HandPiece g_pieceTypeToHandPieceTable[N15_PieceTypeNum];


