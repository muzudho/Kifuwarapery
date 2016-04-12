#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"


class UtilAttack {
public:

	static Bitboard GetAttacksFrom(const PieceType pt, const Color c, const Square sq, const Bitboard& occupied);//inline にするには、positon と utilAttackの依存関係の解消が必要☆（＾ｑ＾）

};