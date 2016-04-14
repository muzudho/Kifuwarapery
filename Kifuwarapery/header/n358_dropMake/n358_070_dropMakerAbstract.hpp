#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"


class DropMakerAbstract {
public:

	virtual void MakeDropMovesToRank9ExceptNL(
		Bitboard& toBB,
		MoveStack* moveStackList,
		PieceType haveHand[6],
		int noKnightLanceIdx
	) const = 0;

	virtual void MakeDropMovesToRank8ExceptN(
		Bitboard& toBB,
		MoveStack* moveStackList,
		PieceType haveHand[6],
		int noKnightIdx
		) const = 0;

	virtual void MakeDropMovesToRank1234567(
		Bitboard& toBB,
		MoveStack* moveStackList,
		PieceType haveHand[6]
		) const = 0;

};


