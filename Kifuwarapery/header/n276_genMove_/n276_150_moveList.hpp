#pragma once

#include "../n226_movStack/n226_105_utilMoveStack.hpp"
#include "../n276_genMove_/n276_100_moveType.hpp"
#include "../n276_genMove_/n276_140_makePromoteMove.hpp"


// MoveType の全ての指し手を生成
template <MoveType MT>
MoveStack* generateMoves(MoveStack* moveStackList, const Position& pos);
template <MoveType MT>
MoveStack* generateMoves(MoveStack* moveStackList, const Position& pos, const Square to);

template <MoveType MT>
class MoveList {
public:

	explicit MoveList(const Position& pos) : m_curr_(m_moveStackList_), m_last_(generateMoves<MT>(m_moveStackList_, pos)) {}

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
