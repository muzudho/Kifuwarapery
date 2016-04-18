#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_205_utilRank.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_230_setMaskBB.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"


class DropMoveGeneratorA {
public:


	static MoveStack* Func001_A_0(
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
		)
	{
		// 桂馬、香車 以外の持ち駒がない。
		return pMovestack;
	}

	static MoveStack* Func001_A_1(
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
		)
	{
		Bitboard toBB = target & TRank9BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_A_2(
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
		)
	{
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

	static MoveStack* Func001_A_3(
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
		)
	{
		Bitboard toBB = target & TRank9BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_A_4(
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
		)
	{
		Bitboard toBB = target & TRank9BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightLanceIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}


	static MoveStack* Func001_B_0(
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
		)
	{
		// 桂馬 以外の持ち駒がない。
		return pMovestack;
	}

	static MoveStack* Func001_B_1(
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
		)
	{
		Bitboard toBB = target & TRank8BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_B_2(
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
		)
	{
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

	static MoveStack* Func001_B_3(
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
		)
	{
		Bitboard toBB = target & TRank8BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_B_4(
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
		)
	{
		Bitboard toBB = target & TRank8BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_B_5(
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
		)
	{
		Bitboard toBB = target & TRank8BB;
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[noKnightIdx + 0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_C_0(
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
		)
	{
		assert(false);// 最適化の為のダミー
		return pMovestack;
	}

	static MoveStack* Func001_C_1(
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
		)
	{
		Bitboard toBB = target & ~(TRank8BB | TRank9BB);
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_C_2(
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
		)
	{
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

	static MoveStack* Func001_C_3(
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
		)
	{
		Bitboard toBB = target & ~(TRank8BB | TRank9BB);
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_C_4(
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
		)
	{
		Bitboard toBB = target & ~(TRank8BB | TRank9BB);
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_C_5(
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
		)
	{
		Bitboard toBB = target & ~(TRank8BB | TRank9BB);
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}

	static MoveStack* Func001_C_6(
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
		)
	{
		Bitboard toBB = target & ~(TRank8BB | TRank9BB);
		do {
			while (toBB.GetP(0)) {
				Square iTo = toBB.PopFirstOneRightFromI9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
			while (toBB.GetP(1)) {
				Square iTo = toBB.PopFirstOneLeftFromB9();
				{
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[5], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[4], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[3], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[2], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[1], iTo); pMovestack++;
					pMovestack->m_move = UtilMove::MakeDropMove(haveHandArr[0], iTo); pMovestack++;
				};
			}
		} while (false);
		return pMovestack;
	}


};