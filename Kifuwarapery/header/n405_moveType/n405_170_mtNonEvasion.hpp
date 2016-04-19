#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_040_mtEvent.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "../n374_genMove_/n374_750_dropMoveGenerator.hpp"
#include "n405_070_mtAbstract.hpp"


class MoveTypeNonEvasion : public MoveTypeAbstract {
public:
	// 部分特殊化
	// 王手が掛かっていないときの指し手生成
	// これには、玉が相手駒の利きのある地点に移動する自殺手と、pin されている駒を動かす自殺手を含む。
	// ここで生成した手は pseudo legal
	MoveStack* GenerateMoves1(
		MoveTypeEvent& mtEvent,
		bool all = false
	) const {
		Bitboard target = mtEvent.m_pos.GetEmptyBB();

		mtEvent.m_moveStackList = g_dropMoveGenerator.GenerateDropMoves(mtEvent, target);//<US>
		target |= mtEvent.m_pos.GetBbOf(mtEvent.m_oppositeColor);
		// 相手玉の位置
		const Square oppositeKsq = mtEvent.m_pos.GetKingSquare(mtEvent.m_oppositeColor);

		PieceMovesEvent pmEvent(
			N07_NonEvasion,
			mtEvent,
			false,
			target,
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


