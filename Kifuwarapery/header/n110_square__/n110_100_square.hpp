#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"

// 盤面を [0, 80] の整数の index で表す
// I9 = 1一, I1 = 1九, A1 = 9九
//
// A9, B9, C9, D9, E9, F9, G9, H9, I9,
// A8, B8, C8, D8, E8, F8, G8, H8, I8,
// A7, B7, C7, D7, E7, F7, G7, H7, I7,
// A6, B6, C6, D6, E6, F6, G6, H6, I6,
// A5, B5, C5, D5, E5, F5, G5, H5, I5,
// A4, B4, C4, D4, E4, F4, G4, H4, I4,
// A3, B3, C3, D3, E3, F3, G3, H3, I3,
// A2, B2, C2, D2, E2, F2, G2, H2, I2,
// A1, B1, C1, D1, E1, F1, G1, H1, I1,
//
// Bitboard のビットが縦に並んでいて、
// 0 ビット目から順に、以下の位置と対応させる。
enum Square {
	I9, I8, I7, I6, I5, I4, I3, I2, I1,
	H9, H8, H7, H6, H5, H4, H3, H2, H1,
	G9, G8, G7, G6, G5, G4, G3, G2, G1,
	F9, F8, F7, F6, F5, F4, F3, F2, F1,
	E9, E8, E7, E6, E5, E4, E3, E2, E1,
	D9, D8, D7, D6, D5, D4, D3, D2, D1,
	C9, C8, C7, C6, C5, C4, C3, C2, C1,
	B9, B8, B7, B6, B5, B4, B3, B2, B1,
	A9, A8, A7, A6, A5, A4, A3, A2, A1,
	SquareNum, // = 81
	SquareNoLeftNum = D9,
	B_hand_pawn = SquareNum + -1,
	B_hand_lance = B_hand_pawn + 18,
	B_hand_knight = B_hand_lance + 4,
	B_hand_silver = B_hand_knight + 4,
	B_hand_gold = B_hand_silver + 4,
	B_hand_bishop = B_hand_gold + 4,
	B_hand_rook = B_hand_bishop + 2,
	W_hand_pawn = B_hand_rook + 2,
	W_hand_lance = W_hand_pawn + 18,
	W_hand_knight = W_hand_lance + 4,
	W_hand_silver = W_hand_knight + 4,
	W_hand_gold = W_hand_silver + 4,
	W_hand_bishop = W_hand_gold + 4,
	W_hand_rook = W_hand_bishop + 2,
	SquareHandNum = W_hand_rook + 3
};
OverloadEnumOperators(Square);
