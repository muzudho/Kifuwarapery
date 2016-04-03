#pragma once

#include "../n113_piece___/n113_155_utilPiece.hpp"


// 持ち駒を表すときに使用する。
// todo: HGold を HRook の後ろに持っていき、PieceType との変換を簡単に出来るようにする。
enum HandPiece {
	HPawn, HLance, HKnight, HSilver, HGold, HBishop, HRook, HandPieceNum
};
OverloadEnumOperators(HandPiece);


const HandPiece PieceTypeToHandPieceTable[PieceTypeNum] = {
	HandPieceNum, HPawn, HLance, HKnight, HSilver, HBishop, HRook, HGold, HandPieceNum, HPawn, HLance, HKnight, HSilver, HBishop, HRook
};


