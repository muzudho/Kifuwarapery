#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "n358_070_dropMakerAbstract.hpp"


class DropMakerHand4 : public DropMakerAbstract {
public:

	void MakeDropMovesToRank9ExceptNL(
		Bitboard& toBB,
		MoveStack* moveStackList,
		PieceType haveHand[6],
		int noKnightLanceIdx
	) const {
		Square to;
		FOREACH_BB(
			toBB,
			to,
			{
				Unroller<4>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightLanceIdx + i], to); });
			}
		);
	}

	void MakeDropMovesToRank8ExceptN(
		Bitboard& toBB,
		MoveStack* moveStackList,
		PieceType haveHand[6],
		int noKnightIdx
	) const {
		Square to;
		FOREACH_BB(
			toBB,
			to,
			{
				Unroller<4>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[noKnightIdx + i], to); });
			}
		);
	}

	void MakeDropMovesToRank1234567(
		Bitboard& toBB,
		MoveStack* moveStackList,
		PieceType haveHand[6]
	) const {
		Square to;
		FOREACH_BB(
			toBB,
			to,
			{
				Unroller<4>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); });
			}
		);
	}

};


extern DropMakerHand4 g_dropMakerHand4;


