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
#include "n358_040_dropMakerEvent.hpp"


class DropMakerAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	virtual MoveStack* MakeDropMovesToRank9ExceptNL(
		DropMakerEvent& dmEvent,
		PieceType haveHandArr[6]
	) const = 0;

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	virtual MoveStack* MakeDropMovesToRank8ExceptN(
		DropMakerEvent& dmEvent,
		PieceType haveHandArr[6]
	) const = 0;

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	virtual MoveStack* MakeDropMovesToRank1234567(
		DropMakerEvent& dmEvent,
		PieceType haveHandArr[6]
	) const = 0;

};


