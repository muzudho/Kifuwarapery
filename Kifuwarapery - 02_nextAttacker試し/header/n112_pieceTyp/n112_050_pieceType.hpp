#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"




enum PieceType {
	// Pro* は 元の 駒の種類に 8 を加算したもの。
	PTPromote = 8,
	N00_Occupied = 0, // 各 PieceType の or をとったもの。
	N01_Pawn,
	N02_Lance,
	N03_Knight,
	N04_Silver,
	N05_Bishop,
	N06_Rook,
	N07_Gold,
	N08_King,
	N09_ProPawn,
	N10_ProLance,
	N11_ProKnight,
	N12_ProSilver,
	N13_Horse,
	N14_Dragon,
	N15_PieceTypeNum,

	N16_GoldHorseDragon // 単にtemnplate引数として使用
};
OverloadEnumOperators(PieceType);
