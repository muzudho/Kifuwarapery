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

	static MoveStack* MakeDropMovesToRank9ExceptNL(
		const Bitboard& target,
		const Bitboard& TRank9BB,
		MoveStack* moveStackList,
		const PieceType haveHand[6],
		int noKnightLanceIdx
	) {
		UNREACHABLE;
		return moveStackList;
	}

	static MoveStack* MakeDropMovesToRank8ExceptN(
		const Bitboard& target,
		const Bitboard& TRank8BB,
		MoveStack* moveStackList,
		const PieceType haveHand[6],
		int noKnightIdx
	) {
		UNREACHABLE;
		return moveStackList;
	}

	static MoveStack* MakeDropMovesToRank1234567(
		Bitboard& toBB,
		MoveStack* moveStackList,
		const PieceType haveHand[6]
	) {
		Square to;
		FOREACH_BB(toBB, to, { Unroller<6>()([&](const int i) { (*moveStackList++).m_move = UtilMove::MakeDropMove(haveHand[i], to); }); });
		//Square to;
		//FOREACH_BB( toBB, to, {
		//	(*pMovestack++).m_move = UtilMove::MakeDropMove(haveHand[5], to);
		//	(*pMovestack++).m_move = UtilMove::MakeDropMove(haveHand[4], to);
		//	(*pMovestack++).m_move = UtilMove::MakeDropMove(haveHand[3], to);
		//	(*pMovestack++).m_move = UtilMove::MakeDropMove(haveHand[2], to);
		//	(*pMovestack++).m_move = UtilMove::MakeDropMove(haveHand[1], to);
		//	(*pMovestack++).m_move = UtilMove::MakeDropMove(haveHand[0], to);
		//});
		return moveStackList;
	}

};
