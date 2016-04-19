﻿#pragma once


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
	MoveStack* GenerateMove(MoveStack* moveStackList, const Position& pos, bool all = false) const {
		Bitboard target = pos.GetEmptyBB();
		Color us = pos.GetTurn();

		moveStackList = g_dropMoveGenerator.GenerateDropMoves(us, moveStackList, pos, target);//<US>
		target |= pos.GetBbOf(UtilColor::OppositeColor(us));
		const Square ksq = pos.GetKingSquare(UtilColor::OppositeColor(us));

		moveStackList = GeneratePieceMoves_N01_Pawn()(N07_NonEvasion, us, false, moveStackList, pos, target, ksq);
		moveStackList = GeneratePieceMoves_N02_Lance()(N07_NonEvasion, us, false, moveStackList, pos, target, ksq);
		moveStackList = GeneratePieceMoves_N03_Knight()(N07_NonEvasion, us, false, moveStackList, pos, target, ksq);
		moveStackList = GeneratePieceMoves_N04_Silver()(N07_NonEvasion, us, false, moveStackList, pos, target, ksq);
		moveStackList = GeneratePieceMoves_N05_Bishop()(N07_NonEvasion, us, false, moveStackList, pos, target, ksq);
		moveStackList = GeneratePieceMoves_N06_Rook()(N07_NonEvasion, us, false, moveStackList, pos, target, ksq);
		moveStackList = GeneratePieceMoves_pt()(N16_GoldHorseDragon, N07_NonEvasion, us, false, moveStackList, pos, target, ksq);
		moveStackList = GeneratePieceMoves_N08_King()(N07_NonEvasion, us, false, moveStackList, pos, target, ksq);

		return moveStackList;
	}

};


