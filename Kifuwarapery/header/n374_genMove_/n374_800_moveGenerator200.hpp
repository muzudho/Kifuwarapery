#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_piece___/n372_070_pieceAbstract.hpp"
#include "../n372_piece___/n372_500_pieceArray.hpp"
#include "n374_050_generateMoves.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"
#include "n374_350_PieceMovesGenerator.hpp"
#include "n374_750_dropMoveGenerator.hpp"
#include "n374_780_MoveGenerator100.hpp""


// MoveType の全ての指し手を生成
class MoveGenerator200 {
public:

	MoveStack* GenerateMoves_2(
		MoveType mt,
		MoveStack* moveStackList, const Position& pos
	) const {
		switch(mt){
		case Capture:
		case NonCapture:
		case CapturePlusPro:
		case NonCaptureMinusPro:
		{
			return g_moveGenerator100.GenerateMoves_mt1(mt, pos.GetTurn(), moveStackList, pos);
		}
		break;
		case Drop:
		{
			return g_moveGenerator100.GenerateMoves_Drop(pos.GetTurn(), moveStackList, pos);
		}
		case Evasion:
		{
			return g_moveGenerator100.GenerateMoves_Evasion(pos.GetTurn(), false/*FIXME:*/, moveStackList, pos);
		}
		case NonEvasion:
		{
			return g_moveGenerator100.GenerateMoves_NonEvasion(pos.GetTurn(), moveStackList, pos);
		}
		break;
		case Legal:
		{
			return g_moveGenerator100.GenerateMoves_Legal(pos.GetTurn(), moveStackList, pos);
		}
		break;
		case LegalAll:
		{
			return g_moveGenerator100.GenerateMoves_LegalAll(pos.GetTurn(), moveStackList, pos);
		}
		break;
		default:
			UNREACHABLE;
			//return g_moveGenerator100.GenerateMoves<MT>()(pos.GetTurn(), moveStackList, pos);
		}
	}


	// pin は省かない。
	MoveStack* GenerateMoves_3(
		MoveStack* moveStackList, const Position& pos, const Square to
	) const {
		const Color us = pos.GetTurn();

		Bitboard fromBB = pos.GetAttackersTo(us, to, pos.GetOccupiedBB());
		while (fromBB.Exists1Bit()) {
			const Square from = fromBB.PopFirstOneFromI9();
			const PieceType pieceType = UtilPiece::ToPieceType(pos.GetPiece(from));

			// TODO: 配列のリミットチェックをしてないぜ☆（＾ｑ＾）
			PieceTypeArray::m_ptArray[pieceType]->Generate2RecaptureMoves(moveStackList, pos, from, to, us);
		}
		return moveStackList;
	}

};


extern MoveGenerator200 g_moveGenerator200;
