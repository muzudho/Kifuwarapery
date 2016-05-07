#pragma once

#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_665_utilMove01.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_080_ptPromote.hpp"
#include "n407_800_moveGenerator200.hpp"


template <MoveType MT>
class MoveList {
public:

	explicit MoveList(const Position& pos) :
		m_curr_(m_moveStackList_), m_last_(g_moveGenerator200.GenerateMoves_2(MT,m_moveStackList_, pos)) {}

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
	MoveStack m_moveStackList_[Move::m_MAX_LEGAL_MOVES];
	MoveStack* m_curr_;
	MoveStack* m_last_;
};
