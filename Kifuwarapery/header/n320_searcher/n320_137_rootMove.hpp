#pragma once

#include "../../header/n223_move____/n223_500_move.hpp"
#include "../../header/n240_position/n240_400_MoveScore.hpp"



class RootMove {
public:
	RootMove() {}
	explicit RootMove(const Move m) : score_(-ScoreInfinite), prevScore_(-ScoreInfinite) {
		pv_.push_back(m);
		pv_.push_back(Move::GetMoveNone());
	}
	explicit RootMove(const MoveScore m) : score_(m.score), prevScore_(-ScoreInfinite) {
		pv_.push_back(m.move);
		pv_.push_back(Move::GetMoveNone());
	}

	bool operator < (const RootMove& m) const {
		return score_ < m.score_;
	}
	bool operator == (const Move& m) const {
		return pv_[0] == m;
	}

	void extractPvFromTT(Position& pos);
	void insertPvInTT(Position& pos);

public:
	Score score_;
	Score prevScore_;
	std::vector<Move> pv_;
};
