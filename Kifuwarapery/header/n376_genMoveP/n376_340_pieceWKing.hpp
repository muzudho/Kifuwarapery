#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n372_genMoveP/n372_070_PieceAbstract.hpp"


class PieceWKing : public PieceAbstract {
public:
	static void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq) {
		UNREACHABLE;
	}

};


//extern PieceWKing g_pieceWKing;
