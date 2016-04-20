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
#include "n374_040_pieceMoveEvent.hpp"


class BishopRookMovesGenerator {
public:

	// 角, 飛車の共通処理☆
	static FORCE_INLINE MoveStack* GenerateBishopOrRookMoves(
		MoveStack* moveStackList,
		const PieceType pt,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
		)
	{
		Bitboard fromBB = ptEvent.m_pos.GetBbOf(pt, ptEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(ptEvent.m_us, UtilSquare::ToRank(from));
			Bitboard toBB = PieceTypeArray::m_ptArray[pt]->GetAttacks2From(ptEvent.m_pos.GetOccupiedBB(), ptEvent.m_us, from) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(ptEvent.m_us, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote) {
					(*moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(ptEvent.m_mt, pt, from, to, ptEvent.m_pos);
					if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all)
						(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(ptEvent.m_mt, pt, from, to, ptEvent.m_pos);
				}
				else // 角、飛車は成れるなら成り、不成は生成しない。
					(*moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(ptEvent.m_mt, pt, from, to, ptEvent.m_pos);
			}
		}
		return moveStackList;
	}

};