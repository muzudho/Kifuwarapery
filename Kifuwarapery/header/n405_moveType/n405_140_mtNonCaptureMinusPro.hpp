#pragma once


#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_040_mtEvent.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "n405_070_mtAbstract.hpp"


class MoveTypeNonCaptureMinusPro : public MoveTypeAbstract {
public:
	// 指し手生成 functor
	// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
	// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
	MoveStack* GenerateMoves1(
		MoveTypeEvent& mtEvent,
		bool all = false
	) const {
		const Bitboard targetPawn = mtEvent.m_pos.GetOccupiedBB().NotThisAnd(mtEvent.m_tRank1_6BB);
		const Bitboard targetOther = mtEvent.m_pos.GetEmptyBB();
		// 相手玉の位置
		const Square oppositeKsq = mtEvent.m_pos.GetKingSquare(mtEvent.m_oppositeColor);

		PieceMovesEvent pmEvent(
			MoveType::N04_NonCaptureMinusPro,
			mtEvent,
			all,
			targetOther,
			oppositeKsq
		);
		// 金、成金、馬、竜の指し手を作る順位を上げてみるぜ☆（＾ｑ＾）
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N16_GoldHorseDragon(pmEvent);
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N01_Pawn(pmEvent);
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N02_Lance(pmEvent);
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N03_Knight(pmEvent);
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N04_Silver(pmEvent);
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N05_Bishop(pmEvent);
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N06_Rook(pmEvent);
		mtEvent.m_moveStackList = PieceMovesGenerator::GeneratePieceMoves_N08_King(pmEvent);

		return mtEvent.m_moveStackList;
	}

};


