﻿#pragma once


#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n374_genMove_/n374_040_mtEvent.hpp"
#include "../n374_genMove_/n374_350_PieceMovesGenerator.hpp"
#include "n405_070_mtAbstract.hpp"
#include "n405_160_mtEvasion.hpp"
#include "n405_170_mtNonEvasion.hpp"


class MoveTypeLegalAll : public MoveTypeAbstract {
public:
	// 部分特殊化
	// Evasion のときに歩、飛、角と、香の2段目の不成も生成する。
	MoveStack* GenerateMoves1(
		MoveTypeEvent& mtEvent,
		bool all = false
	) const {
		MoveStack* curr = mtEvent.m_moveStackList;
		const Bitboard pinned = mtEvent.m_pos.GetPinnedBB();

		mtEvent.m_moveStackList = mtEvent.m_pos.InCheck() ?
			MoveTypeEvasion().GenerateMoves1(mtEvent, true) :
			MoveTypeNonEvasion().GenerateMoves1(mtEvent);

		// 玉の移動による自殺手と、pinされている駒の移動による自殺手を削除
		while (curr != mtEvent.m_moveStackList) {
			if (!mtEvent.m_pos.IsPseudoLegalMoveIsLegal<false, false>(curr->m_move, pinned)) {
				curr->m_move = (--mtEvent.m_moveStackList)->m_move;
			}
			else {
				++curr;
			}
		}

		return mtEvent.m_moveStackList;
	}

};


