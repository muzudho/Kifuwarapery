#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_150_piece.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_piece___/n372_070_pieceAbstract.hpp"
#include "../n372_piece___/n372_500_pieceArray.hpp"
#include "../n374_genMove_/n374_050_generateMoves.hpp"
#include "../n374_genMove_/n374_250_bishopRookMovesGenerator.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "../n374_genMove_/n374_750_dropMoveGenerator.hpp"
#include "../n374_genMove_/n374_780_MoveGenerator100.hpp""
#include "../n405_moveType/n405_100_mtCapture.hpp"
#include "../n405_moveType/n405_110_mtNonCapture.hpp"
#include "../n405_moveType/n405_120_mtDrop.hpp"
#include "../n405_moveType/n405_130_mtCapturePlusPro.hpp"
#include "../n405_moveType/n405_140_mtNonCaptureMinusPro.hpp"
#include "../n405_moveType/n405_150_mtRecapture.hpp"
#include "../n405_moveType/n405_160_mtEvasion.hpp"
#include "../n405_moveType/n405_170_mtNonEvasion.hpp"
#include "../n405_moveType/n405_180_mtLegal.hpp"
#include "../n405_moveType/n405_190_mtLegalAll.hpp"
#include "../n405_moveType/n405_200_mtMoveTypeNone.hpp"
#include "../n405_moveType/n405_500_mtArray.hpp"


// MoveType の全ての指し手を生成
class MoveGenerator200 {
public:

	inline MoveStack* GenerateMoves_2(
		MoveType mt, MoveStack* moveStackList, const Position& pos
	) const {
		return g_moveTypeArray.m_moveTypeArray[mt]->GenerateMove(moveStackList, pos);
	}


	// pin は省かない。リキャプチャー専用か。
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
