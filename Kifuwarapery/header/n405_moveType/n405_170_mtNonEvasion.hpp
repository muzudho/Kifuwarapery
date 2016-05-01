#pragma once


#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "../n374_genMove_/n374_750_dropMoveGenerator.hpp"
#include "n405_070_mtAbstract.hpp"


class MoveTypeNonEvasion : public MoveTypeAbstract {
public:
	// 部分特殊化
	// 王手が掛かっていないときの指し手生成
	// これには、玉が相手駒の利きのある地点に移動する自殺手と、pin されている駒を動かす自殺手を含む。
	// ここで生成した手は pseudo legal
	MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all = false
		) const override {

		if (pos.GetTurn() == Color::Black)
		{
			moveStackList = MoveTypeNonEvasion::GENERATE_MOVE_<Color::Black, Color::White>(moveStackList, pos, all);
		}
		else
		{
			moveStackList = MoveTypeNonEvasion::GENERATE_MOVE_<Color::White, Color::Black>(moveStackList, pos, all);
		}

		return moveStackList;
	}

private:

	template<Color US, Color THEM>
	static inline MoveStack* GENERATE_MOVE_(MoveStack* moveStackList, const Position& pos, bool all = false
		) {

		Bitboard target = pos.GetEmptyBB();

		moveStackList = g_dropMoveGenerator.GenerateDropMoves<US,THEM>(moveStackList, pos, target);
		target |= pos.GetBbOf10(THEM);
		const Square ksq = pos.GetKingSquare(THEM);

		const PieceMoveEvent pmEvent(MoveType::N07_NonEvasion, false, pos, ksq);

		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N01_Pawn<US>(moveStackList, pmEvent, target);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N02_Lance<US>(moveStackList, pmEvent, target);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N03_Knight<US>(moveStackList, pmEvent, target);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N04_Silver<US>(moveStackList, pmEvent, target);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N05_Bishop<US>(moveStackList, pmEvent, target);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N06_Rook<US>(moveStackList, pmEvent, target);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N16_GoldHorseDragon<US>(moveStackList, pmEvent, target);
		moveStackList = PieceMovesGenerator::GeneratePieceMoves_N08_King<US>(moveStackList, pmEvent, target);

		return moveStackList;
	}

};


