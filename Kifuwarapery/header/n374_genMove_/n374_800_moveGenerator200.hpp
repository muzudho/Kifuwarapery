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


class MoveGenerator200 {
public:

	template <MoveType MT>
	static MoveStack* GenerateMoves_2(
		MoveStack* moveStackList, const Position& pos
	) {
		return MoveGenerator100::GenerateMoves<MT>()(pos.GetTurn(), moveStackList, pos);
	}


	// pin は省かない。
	//template <MoveType MT>
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