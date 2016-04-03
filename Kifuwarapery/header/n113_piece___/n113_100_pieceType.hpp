#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"




enum PieceType {
	// Pro* �� ���� ��̎�ނ� 8 �����Z�������́B
	PTPromote = 8,
	Occupied = 0, // �e PieceType �� or ���Ƃ������́B
	Pawn, Lance, Knight, Silver, Bishop, Rook, Gold, King,
	ProPawn, ProLance, ProKnight, ProSilver, Horse, Dragon,
	PieceTypeNum,

	GoldHorseDragon // �P��temnplate�����Ƃ��Ďg�p
};
OverloadEnumOperators(PieceType);
