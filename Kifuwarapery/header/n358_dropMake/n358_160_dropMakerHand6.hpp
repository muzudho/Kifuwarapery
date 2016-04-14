#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "n358_070_dropMakerAbstract.hpp"


class DropMakerHand6 : public DropMakerAbstract {
public:

	void MakeDropMovesToRank9ExceptNL(
		const Bitboard& target,
		const Bitboard TRank9BB,
		MoveStack* pMoveStackList,
		const PieceType haveHand[6],
		int noKnightLanceIdx
	) const {
		UNREACHABLE;
	}

	void MakeDropMovesToRank8ExceptN(
		const Bitboard& target,
		const Bitboard TRank8BB,
		MoveStack* pMoveStackList,
		const PieceType haveHand[6],
		int noKnightIdx
	) const {
		UNREACHABLE;
	}

	void MakeDropMovesToRank1234567(
		Bitboard& toBB,
		MoveStack* pMoveStackList,
		const PieceType haveHand[6]
	) const {
		Square to;
		FOREACH_BB( toBB, to, {
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
			(*pMoveStackList++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
		});
	}

};


extern DropMakerHand6 g_dropMakerHand6;


