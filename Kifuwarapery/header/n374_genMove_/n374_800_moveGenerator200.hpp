#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_genMoveP/n372_070_pieceAbstract.hpp"
#include "n374_050_generateMoves.hpp"
#include "n374_250_bishopRookMovesGenerator.hpp"
#include "n374_350_PieceMovesGenerator.hpp"
#include "n374_650_bannedKingToMaker.hpp"
#include "n374_750_dropMoveGenerator.hpp"
#include "n374_780_MoveGenerator100.hpp""
#include "../n376_genMoveP/n376_500_pieceArray.hpp"


// MoveType の全ての指し手を生成
class MoveGenerator200 {
public:

	static MoveStack* GenerateMoves_2(
		MoveType mt,
		MoveStack* moveStackList, const Position& pos
	) {
		switch(mt){
		case Capture:
		case NonCapture:
		case CapturePlusPro:
		case NonCaptureMinusPro:
		{
			return MoveGenerator100::GenerateMoves_mt1(mt, pos.GetTurn(), moveStackList, pos);
		}
		break;
		case Drop:
		{
			return MoveGenerator100::GenerateMoves_Drop(pos.GetTurn(), moveStackList, pos);
		}
		case Evasion:
		{
			return MoveGenerator100::GenerateMoves_Evasion(pos.GetTurn(), false/*FIXME:*/, moveStackList, pos);
		}
		case NonEvasion:
		{
			return MoveGenerator100::GenerateMoves_NonEvasion(pos.GetTurn(), moveStackList, pos);
		}
		break;
		case Legal:
		{
			return MoveGenerator100::GenerateMoves_Legal(pos.GetTurn(), moveStackList, pos);
		}
		break;
		case LegalAll:
		{
			return MoveGenerator100::GenerateMoves_LegalAll(pos.GetTurn(), moveStackList, pos);
		}
		break;
		default:
			UNREACHABLE;
			//return MoveGenerator100::GenerateMoves<MT>()(pos.GetTurn(), moveStackList, pos);
		}
	}


	// pin は省かない。
	static MoveStack* GenerateMoves_3(
		MoveStack* moveStackList, const Position& pos, const Square to
	) {
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