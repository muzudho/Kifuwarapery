#pragma once

#include "../n113_piece___/n113_100_pieceType.hpp"
#include "../n113_piece___/n113_200_handPiece.hpp"

//#include "../n116_hand____/n116_500_hand.hpp" // TODO:
//class Hand;


// 持ち駒を表すときに使用する。
// todo: HGold を HRook の後ろに持っていき、PieceType との変換を簡単に出来るようにする。
enum HandPiece {
	HPawn, HLance, HKnight, HSilver, HGold, HBishop, HRook, HandPieceNum
};
OverloadEnumOperators(HandPiece);


extern const HandPiece g_pieceTypeToHandPieceTable[PieceTypeNum];


