#pragma once

#include "../n240_position/n240_160_moveStack.hpp"
#include "../n276_genMove_/n276_100_moveType.hpp"
#include "../n276_genMove_/n276_250_makePromoteMove.hpp"


// MoveType の全ての指し手を生成
template <MoveType MT>
MoveStack* generateMoves(MoveStack* moveStackList, const Position& pos);
template <MoveType MT>
MoveStack* generateMoves(MoveStack* moveStackList, const Position& pos, const Square to);

template <MoveType MT>
class MoveList {
public:
	explicit MoveList(const Position& pos) : curr_(moveStackList_), last_(generateMoves<MT>(moveStackList_, pos)) {}
	void operator ++ () { ++curr_; }
	bool end() const { return (curr_ == last_); }
	Move move() const { return curr_->move; }
	size_t size() const { return static_cast<size_t>(last_ - moveStackList_); }
	bool contains(const Move move) const {
		for (const MoveStack* it(moveStackList_); it != last_; ++it) {
			if (it->move == move) {
				return true;
			}
		}
		return false;
	}

private:
	MoveStack moveStackList_[MaxLegalMoves];
	MoveStack* curr_;
	MoveStack* last_;
};
