#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_205_convRank.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_230_setMaskBB.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "n358_070_dropMakerAbstract.hpp"


class DropMakerHand6 : public DropMakerAbstract {
public:

	MoveStack* MakeDropMovesToRank9ExceptNL(
		MoveStack* pMovestack,
		const DropMakerEvent& dmEvent,
		Move haveHandArr[6]
	) const override {
		// このメソッドを実行しているようではエラーだぜ☆（＾ｑ＾）
		UNREACHABLE;
		return pMovestack;
	}

	MoveStack* MakeDropMovesToRank8ExceptN(
		MoveStack* pMovestack,
		const DropMakerEvent& dmEvent,
		Move haveHandArr[6]
	) const override {
		// このメソッドを実行しているようではエラーだぜ☆（＾ｑ＾）
		UNREACHABLE;
		return pMovestack;
	}

	MoveStack* MakeDropMovesToRank1234567(
		MoveStack* pMovestack,
		const DropMakerEvent& dmEvent,
		Move haveHandArr[6]
	) const override {
		Bitboard toBB = dmEvent.m_target & ~(dmEvent.m_tRank8BB | dmEvent.m_tRank9BB);
		while (toBB.GetP(0)) {
			Square iTo = toBB.PopFirstOneRightFromI9();
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[5], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[4], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[3], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[2], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[1], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[0], iTo);
			pMovestack++;
		}
		while (toBB.GetP(1)) {
			Square iTo = toBB.PopFirstOneLeftFromB9();
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[5], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[4], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[3], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[2], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[1], iTo);
			pMovestack++;
			pMovestack->m_move = ConvMove::Convert30_MakeDropMove_da(haveHandArr[0], iTo);
			pMovestack++;
		}
		return pMovestack;
	}

};
