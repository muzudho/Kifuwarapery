#pragma once


#include "../../n080_common__/n080_100_common.hpp"
#include "../../n105_color___/n105_100_color.hpp"
#include "../../n110_square__/n110_100_square.hpp"
#include "../../n110_square__/n110_500_convSquare.hpp"
#include "../../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../n160_board___/n160_100_bitboard.hpp"
#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_040_ptEvent.hpp"
#include "../../n300_moveGen_/n300_200_pieceTyp/n300_200_500_ptPrograms.hpp"
#include "n300_400_040_pieceMoveEvent.hpp"


class BishopRookMovesGenerator {
public:

	// 角, 飛車の共通処理☆
	template<const bool isBishop,const Color US>
	static FORCE_INLINE MoveStack* GenerateBishopOrRookMoves(
		MoveStack* moveStackList,
		//const PieceType pt,
		Move pieceTypeAsMove,
		const PieceMoveEvent ptEvent,
		const Bitboard& target
		)
	{
		Bitboard fromBB = ptEvent.m_pos.GetBbOf20<US>(isBishop ? PieceType::N05_Bishop : PieceType::N06_Rook);
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const bool fromCanPromote = ConvSquare::CAN_PROMOTE10<US>(ConvSquare::TO_RANK10(from));
			const PieceTypeEvent ptEvent1(ptEvent.m_pos.GetOccupiedBB(), US, from);
			Bitboard toBB = PiecetypePrograms::m_PIECETYPE_PROGRAMS[(isBishop?PieceType::N05_Bishop:PieceType::N06_Rook)]->GetAttacks2From(ptEvent1) & target;
			while (toBB.Exists1Bit()) {
				const Square to = toBB.PopFirstOneFromI9();
				const bool toCanPromote = ConvSquare::CAN_PROMOTE10<US>(ConvSquare::TO_RANK10(to));
				if (fromCanPromote | toCanPromote) {

					// 成りVer☆
					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,pieceTypeAsMove,from, to, ptEvent.m_pos);
					MakePromoteMove::APPEND_PROMOTE_FLAG(moveStackList->m_move);
					moveStackList++;

					// 次の指し手は不成Ver☆
					if (ptEvent.m_mt == N07_NonEvasion || ptEvent.m_all)
					{
						moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,pieceTypeAsMove,from, to, ptEvent.m_pos);
						moveStackList++;
					}
				}
				else // 角、飛車は成れるなら成り、不成は生成しない。
				{
					moveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_mt2(ptEvent.m_mt,pieceTypeAsMove,from, to, ptEvent.m_pos);
					moveStackList++;
				}
			}
		}
		return moveStackList;
	}

};