#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "n358_070_dropMakerAbstract.hpp"


class DropMakerHand0 : public DropMakerAbstract {
public:
	static MoveStack* MakeDropMovesToRank9ExceptNL(
		const Bitboard& target,
		const Bitboard& TRank9BB,
		MoveStack* pMovestack,
		const PieceType haveHand[6],
		int noKnightLanceIdx
	) {
		// 桂馬、香車 以外の持ち駒がない。
		return pMovestack;
	}

	static MoveStack* MakeDropMovesToRank8ExceptN(
		const Bitboard& target,
		const Bitboard& TRank8BB,
		MoveStack* pMovestack,
		const PieceType haveHand[6],
		int noKnightIdx
	) {
		// 桂馬 以外の持ち駒がない。
		return pMovestack;
	}

	static MoveStack* MakeDropMovesToRank1234567(
		Bitboard& toBB,
		MoveStack* pMovestack,
		const PieceType haveHand[6]
		) {
		assert(false); // 最適化の為のダミー
		return pMovestack;
	}

};

