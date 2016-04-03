#pragma once

#include "../n113_piece___/n113_155_utilPiece.hpp"


// �������\���Ƃ��Ɏg�p����B
// todo: HGold �� HRook �̌��Ɏ����Ă����APieceType �Ƃ̕ϊ����ȒP�ɏo����悤�ɂ���B
enum HandPiece {
	HPawn, HLance, HKnight, HSilver, HGold, HBishop, HRook, HandPieceNum
};
OverloadEnumOperators(HandPiece);


const HandPiece PieceTypeToHandPieceTable[PieceTypeNum] = {
	HandPieceNum, HPawn, HLance, HKnight, HSilver, HBishop, HRook, HGold, HandPieceNum, HPawn, HLance, HKnight, HSilver, HBishop, HRook
};


