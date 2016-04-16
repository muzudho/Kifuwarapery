#pragma once

#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n360_genMOve_/n360_800_moveGenerator200.hpp"

/*
// MoveType の全ての指し手を生成
template <MoveType MT>
MoveStack* MoveGenerator200::GenerateMoves_2(MoveStack* moveStackList, const Position& pos);
template <MoveType MT>
MoveStack* MoveGenerator200::GenerateMoves_3(MoveStack* moveStackList, const Position& pos, const Square to);
*/


template <MoveType MT>
class MoveList {
public:

	explicit MoveList(const Position& pos) :
		m_curr_(m_moveStackList_), m_last_(MoveGenerator200::GenerateMoves_2<MT>(m_moveStackList_, pos)) {}

	void operator ++ () { ++m_curr_; }

	bool IsEnd() const { return (m_curr_ == m_last_); }

	Move GetMove() const { return m_curr_->m_move; }
	size_t GetSize() const { return static_cast<size_t>(m_last_ - m_moveStackList_); }
	bool Contains(const Move move) const {
		for (const MoveStack* it(m_moveStackList_); it != m_last_; ++it) {
			if (it->m_move == move) {
				return true;
			}
		}
		return false;
	}

private:
	MoveStack m_moveStackList_[g_MaxLegalMoves];
	MoveStack* m_curr_;
	MoveStack* m_last_;
};
