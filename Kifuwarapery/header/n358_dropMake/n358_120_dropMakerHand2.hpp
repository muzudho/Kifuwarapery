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


class DropMakerHand2 : public DropMakerAbstract {
public:

	static MoveStack* MakeDropMovesToRank9ExceptNL(
		Color us,
		MoveStack* pMovestack,
		const Position& pos,
		const Bitboard& target,
		const Hand& hand,
		const int haveHandNum,
		const int noKnightIdx,
		const int noKnightLanceIdx,
		const Bitboard& TRank8BB,
		const Bitboard& TRank9BB,
		PieceType haveHandArr[6]
	) {
		Bitboard toBB = target & TRank9BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* MakeDropMovesToRank8ExceptN(
		Color us,
		MoveStack* pMovestack,
		const Position& pos,
		const Bitboard& target,
		const Hand& hand,
		const int haveHandNum,
		const int noKnightIdx,
		const int noKnightLanceIdx,
		const Bitboard& TRank8BB,
		const Bitboard& TRank9BB,
		PieceType haveHandArr[6]
	) {
		Bitboard toBB = target & TRank8BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* MakeDropMovesToRank1234567(
		Color us,
		MoveStack* pMovestack,
		const Position& pos,
		const Bitboard& target,
		const Hand& hand,
		const int haveHandNum,
		const int noKnightIdx,
		const int noKnightLanceIdx,
		const Bitboard& TRank8BB,
		const Bitboard& TRank9BB,
		PieceType haveHandArr[6]
	) {
		Bitboard toBB = target & ~(TRank8BB | TRank9BB);
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

};


