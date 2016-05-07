#pragma once


#include "../../n110_square__/n110_100_square.hpp"
#include "../../n160_board___/n160_100_bitboard.hpp"
#include "../../n160_board___/n160_140_goldAttackBb.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "n300_500_070_pieceAbstract.hpp"


class PieceBProLance : public PieceAbstract {
public:
	void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq
		) const override {
		bannedKingToBB |= g_goldAttackBb.GetControllBb(Color::Black, checkSq);
	}
};
