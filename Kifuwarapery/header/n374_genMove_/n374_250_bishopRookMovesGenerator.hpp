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
#include "n374_045_pmEvent.hpp""


class BishopRookMovesGenerator {
public:

	// 角, 飛車の共通処理☆
	static FORCE_INLINE MoveStack* GenerateBishopOrRookMoves(
		PieceType pt,
		PieceMovesEvent pmEvent
		/*
		MoveType mt,
		MoveTypeEvent& mtEvent,
		bool all,
		const Bitboard& target,
		const Square //ksq
		*/
	)
	{
		Bitboard fromBB = pmEvent.m_mtEvent.m_pos.GetBbOf(pt, pmEvent.m_mtEvent.m_us);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = UtilSquare::CanPromote(pmEvent.m_mtEvent.m_us, UtilSquare::ToRank(from));
			Bitboard toBB = PieceTypeArray::m_ptArray[pt]->GetAttacks2From(pmEvent.m_mtEvent.m_pos.GetOccupiedBB(), pmEvent.m_mtEvent.m_us, from) & pmEvent.m_target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = UtilSquare::CanPromote(pmEvent.m_mtEvent.m_us, UtilSquare::ToRank(to));
				if (fromCanPromote | toCanPromote) {
					(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakePromoteMove2(pmEvent.m_mt, pt, from, to, pmEvent.m_mtEvent.m_pos);
					if (pmEvent.m_mt == N07_NonEvasion || pmEvent.m_all)
						(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(
							pmEvent.m_mt, pt, from, to, pmEvent.m_mtEvent.m_pos);
				}
				else // 角、飛車は成れるなら成り、不成は生成しない。
					(*pmEvent.m_mtEvent.m_moveStackList++).m_move = g_makePromoteMove.MakeNonPromoteMove(pmEvent.m_mt, pt, from, to, pmEvent.m_mtEvent.m_pos);
			}
		}
		return pmEvent.m_mtEvent.m_moveStackList;
	}

};