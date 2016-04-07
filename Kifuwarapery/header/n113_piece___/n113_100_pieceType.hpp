#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"




enum PieceType {
	// Pro* は 元の 駒の種類に 8 を加算したもの。
	PTPromote = 8,
	Occupied = 0, // 各 PieceType の or をとったもの。
	Pawn, Lance, Knight, Silver, Bishop, Rook, Gold, King,
	ProPawn, ProLance, ProKnight, ProSilver, Horse, Dragon,
	PieceTypeNum,

	GoldHorseDragon // 単にtemnplate引数として使用
};
OverloadEnumOperators(PieceType);
