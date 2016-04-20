#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_205_utilRank.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_230_setMaskBB.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "n358_070_dropMakerAbstract.hpp"


class DropMakerHand0 : public DropMakerAbstract {
public:
	MoveStack* MakeDropMovesToRank9ExceptNL(
		MoveStack* pMovestack,
		DropMakerEvent& dmEvent,
		PieceType haveHandArr[6]
	) const  {
		// 桂馬、香車 以外の持ち駒がない。
		return pMovestack;
	}

	MoveStack* MakeDropMovesToRank8ExceptN(
		MoveStack* pMovestack,
		DropMakerEvent& dmEvent,
		PieceType haveHandArr[6]
	) const {
		// 桂馬 以外の持ち駒がない。
		return pMovestack;
	}

	MoveStack* MakeDropMovesToRank1234567(
		MoveStack* pMovestack,
		DropMakerEvent& dmEvent,
		PieceType haveHandArr[6]
	) const {
		assert(false);// 最適化の為のダミー
		return pMovestack;
	}

};

