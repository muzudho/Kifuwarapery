#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_500_utilSquare.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"


class BishopRookMovesGenerator {
public:

	// 角, 飛車の共通処理☆
	static FORCE_INLINE MoveStack* GenerateBishopOrRookMoves(
		MoveType mt,
		PieceType pt,
		Color us,
		bool all,
		MoveStack* moveStackList, const Position& pos, const Bitboard& target, const Square /*ksq*/
		)
	{
		Bitboard fromBB = pos.GetBbOf(pt, us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(us, UtilSquare::ToRank(from));
			Bitboard toBB = PieceTypeArray::m_ptArray[pt]->GetAttacks2From(pos.GetOccupiedBB(), us, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(us, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote) {
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(mt, pt, from, to, pos);
					if (mt == NonEvasion || all)
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, pt, from, to, pos);
				}
				else // 角、飛車は成れるなら成り、不成は生成しない。
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(mt, pt, from, to, pos);
			}
		}
		return moveStackList;
	}

};