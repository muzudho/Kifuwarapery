#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_205_convRank.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_230_setMaskBB.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_420_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_020_moveStack.hpp"
#include "n358_040_dropMakerEvent.hpp"


class DropMakerAbstract {
public:

	virtual MoveStack* MakeDropMovesToRank9ExceptNL(
		MoveStack* pMovestack,
		const DropMakerEvent& dmEvent,
		Move haveHandArr[6] // TODO: これ打ムーブにできないかだぜ☆？（＾ｑ＾）
	) const = 0;

	virtual MoveStack* MakeDropMovesToRank8ExceptN(
		MoveStack* pMovestack,
		const DropMakerEvent& dmEvent,
		Move haveHandArr[6] // TODO: これ打ムーブにできないかだぜ☆？（＾ｑ＾）
	) const = 0;

	virtual MoveStack* MakeDropMovesToRank1234567(
		MoveStack* pMovestack,
		const DropMakerEvent& dmEvent,
		Move haveHandArr[6] // TODO: これ打ムーブにできないかだぜ☆？（＾ｑ＾）
	) const = 0;

};


