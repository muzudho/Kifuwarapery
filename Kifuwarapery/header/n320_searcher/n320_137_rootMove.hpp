#pragma once

#include "../n223_move____/n223_105_utilMove.hpp"
#include "../n223_move____/n223_300_moveScore.hpp"



class RootMove {
public:

	RootMove() {}

	explicit RootMove(const Move m) : m_score_(-ScoreInfinite), m_prevScore_(-ScoreInfinite) {
		m_pv_.push_back(m);
		m_pv_.push_back(Move::GetMoveNone());
	}

	explicit RootMove(const MoveScore m) : m_score_(m.score), m_prevScore_(-ScoreInfinite) {
		m_pv_.push_back(m.move);
		m_pv_.push_back(Move::GetMoveNone());
	}

	bool operator < (const RootMove& m) const {
		return m_score_ < m.m_score_;
	}
	bool operator == (const Move& m) const {
		return m_pv_[0] == m;
	}

	void ExtractPvFromTT(Position& pos);

	void InsertPvInTT(Position& pos);

public:
	Score m_score_;
	Score m_prevScore_;
	std::vector<Move> m_pv_;
};
