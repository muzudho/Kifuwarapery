#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_130_lanceAttackBb.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n372_piece___/n372_070_PieceAbstract.hpp"


class PieceWLance : public PieceAbstract {
public:
	void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq
		) const override {
		bannedKingToBB |= g_lanceAttackBb.GetControllBbToEdge(Color::White, checkSq);
	}
};
