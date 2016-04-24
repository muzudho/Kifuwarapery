#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_200_horseAttackBb.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n372_piece___/n372_070_PieceAbstract.hpp"


class PieceBHorse : public PieceAbstract {
public:
	void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq) const {
		bannedKingToBB |= g_horseAttackBb.GetControllBbToEdge(checkSq);
	}
};
