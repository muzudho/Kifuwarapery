#pragma once


#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "n405_070_mtAbstract.hpp"


class MoveTypeCapturePlusPro : public MoveTypeAbstract {
public:
	// 指し手生成 functor
	// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
	// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
	MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all = false) const {
		MoveType MT = N03_CapturePlusPro;
		Color us = pos.GetTurn();

		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (us == Black ? Rank6 : Rank4);
		const Rank TRank7 = (us == Black ? Rank7 : Rank3);
		const Rank TRank8 = (us == Black ? Rank8 : Rank2);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(us, TRank6);
		const Bitboard TRank1_6BB = g_inFrontMaskBb.GetInFrontMask(UtilColor::OppositeColor(us), TRank7);
		const Bitboard TRank1_7BB = g_inFrontMaskBb.GetInFrontMask(UtilColor::OppositeColor(us), TRank8);

		const Bitboard targetPawn = pos.GetBbOf10(UtilColor::OppositeColor(us)) | (pos.GetOccupiedBB().NotThisAnd(TRank789BB));
		const Bitboard targetOther = pos.GetBbOf10(UtilColor::OppositeColor(us));
		const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(us));

		const PieceMoveEvent pmEvent(MT, us, all, pos, ksq);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N01_Pawn( moveStackList, pmEvent, targetPawn);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N02_Lance(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N03_Knight(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N04_Silver(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N05_Bishop(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N06_Rook(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N16_GoldHorseDragon(moveStackList, pmEvent, targetOther);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N08_King(moveStackList, pmEvent, targetOther);

		return moveStackList;
	}

};


