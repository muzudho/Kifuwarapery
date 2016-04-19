#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n372_genMoveP/n372_070_PieceAbstract.hpp"


class PieceBPawn : public PieceAbstract {
public:
	void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq) const {
		// 歩、桂馬で王手したときは、どこへ逃げても、その駒で取られることはない。
		// よって、ここでは何もしない。
		assert(pos.GetPiece(checkSq) == N01_BPawn || pos.GetPiece(checkSq) == N03_BKnight);
	}

};

