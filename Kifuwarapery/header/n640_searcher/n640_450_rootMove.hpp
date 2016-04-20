#pragma once

#include <vector>
#include "../n165_movStack/n165_400_move.hpp"
#include "../n223_move____/n223_300_moveAndScore.hpp"


#include "n640_440_splitedNode.hpp"


class RootMove {
public:

	RootMove() {}

	explicit RootMove(const Move m) : m_score_(-ScoreInfinite), m_prevScore_(-ScoreInfinite) {
		m_pv_.push_back(m);
		m_pv_.push_back(Move::GetMoveNone());
	}

	explicit RootMove(const MoveAndScore m) : m_score_(m.m_score), m_prevScore_(-ScoreInfinite) {
		m_pv_.push_back(m.m_move);
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

	// 評価値
	Score m_score_;

	// 前の評価値
	Score m_prevScore_;

	// 指し手のスタック
	std::vector<Move> m_pv_;

};
