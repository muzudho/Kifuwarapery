#pragma once


#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "../n374_genMove_/n374_750_dropMoveGenerator.hpp"
#include "n405_040_mtEvent.hpp"
#include "n405_070_mtAbstract.hpp"


class MoveTypeDrop : public MoveTypeAbstract {
public:
	// 部分特殊化
	// 駒打ち生成
	MoveStack* GenerateMoves1(
		MoveTypeEvent& mtEvent,
		bool all = false
	) const {
		const Bitboard target = mtEvent.m_pos.GetEmptyBB();

		mtEvent.m_moveStackList = g_dropMoveGenerator.GenerateDropMoves(
			mtEvent.m_us, mtEvent.m_moveStackList, mtEvent.m_pos, target);//<US>
		return mtEvent.m_moveStackList;
	}

};


