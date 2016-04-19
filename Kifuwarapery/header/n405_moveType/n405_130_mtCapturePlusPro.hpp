﻿#pragma once


#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_040_mtEvent.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "n405_070_mtAbstract.hpp"


class MoveTypeCapturePlusPro : public MoveTypeAbstract {
public:
	// 指し手生成 functor
	// テンプレート引数が複数あり、部分特殊化したかったので、関数ではなく、struct にした。
	// ALL == true のとき、歩、飛、角の不成、香の2段目の不成、香の3段目の駒を取らない不成も生成する。
	MoveStack* GenerateMoves1(
		MoveTypeEvent& mtEvent,
		bool all = false
	) const {
		MoveType MT = N03_CapturePlusPro;

		// Txxx は先手、後手の情報を吸収した変数。数字は先手に合わせている。
		const Rank TRank6 = (mtEvent.m_us == Black ? Rank6 : Rank4);
		const Rank TRank7 = (mtEvent.m_us == Black ? Rank7 : Rank3);
		const Rank TRank8 = (mtEvent.m_us == Black ? Rank8 : Rank2);
		const Bitboard TRank789BB = g_inFrontMaskBb.GetInFrontMask(mtEvent.m_us, TRank6);
		const Bitboard TRank1_6BB = g_inFrontMaskBb.GetInFrontMask(mtEvent.m_oppositeColor, TRank7);
		const Bitboard TRank1_7BB = g_inFrontMaskBb.GetInFrontMask(mtEvent.m_oppositeColor, TRank8);

		const Bitboard targetPawn = mtEvent.m_pos.GetBbOf(mtEvent.m_oppositeColor) | (mtEvent.m_pos.GetOccupiedBB().NotThisAnd(TRank789BB));
		const Bitboard targetOther = mtEvent.m_pos.GetBbOf(mtEvent.m_oppositeColor);
		const Square ksq = mtEvent.m_pos.GetKingSquare(mtEvent.m_oppositeColor);

		mtEvent.m_moveStackList = GeneratePieceMoves_N01_Pawn(MT, mtEvent, all, targetPawn, ksq);
		mtEvent.m_moveStackList = GeneratePieceMoves_N02_Lance(MT, mtEvent, all, targetOther, ksq);
		mtEvent.m_moveStackList = GeneratePieceMoves_N03_Knight(MT, mtEvent, all, targetOther, ksq);
		mtEvent.m_moveStackList = GeneratePieceMoves_N04_Silver(MT, mtEvent, all, targetOther, ksq);
		mtEvent.m_moveStackList = GeneratePieceMoves_N05_Bishop(MT, mtEvent, all, targetOther, ksq);
		mtEvent.m_moveStackList = GeneratePieceMoves_N06_Rook(MT, mtEvent, all, targetOther, ksq);
		mtEvent.m_moveStackList = GeneratePieceMoves_pt(N16_GoldHorseDragon, MT, mtEvent, all, targetOther, ksq);
		mtEvent.m_moveStackList = GeneratePieceMoves_N08_King(MT, mtEvent, all, targetOther, ksq);

		return mtEvent.m_moveStackList;
	}

};


