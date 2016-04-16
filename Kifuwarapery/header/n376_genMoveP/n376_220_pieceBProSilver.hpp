﻿#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_140_goldAttackBb.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n372_genMoveP/n372_070_PieceAbstract.hpp"


class PieceBProSilver : public PieceAbstract {
public:
	static void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq) {
		bannedKingToBB |= g_goldAttackBb.GetControllBb(Color::Black, checkSq);
	}

};


//extern PieceBProSilver g_pieceBProSilver;