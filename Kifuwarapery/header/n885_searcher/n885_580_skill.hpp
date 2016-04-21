#pragma once


#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n885_searcher/n885_500_rucksack.hpp"


// 何がスキルなのか☆？（＾ｑ＾）
struct Skill {

	Skill(const int l, const int mr)
		: m_level(l),
		m_maxRandomScoreDiff(static_cast<ScoreIndex>(mr)),
		m_best(g_MOVE_NONE) {}

	~Skill() {}

	void swapIfEnabled(Rucksack* s) {
		if (enabled()) {
			auto it = std::find(s->m_rootMoves.begin(),
				s->m_rootMoves.end(),
				(!m_best.IsNone() ? m_best : pickMove(s)));
			if (s->m_rootMoves.begin() != it)
				SYNCCOUT << "info string swap multipv 1, " << it - s->m_rootMoves.begin() + 1 << SYNCENDL;
			std::swap(s->m_rootMoves[0], *it);
		}
	}

	bool enabled() const { return m_level < 20 || m_maxRandomScoreDiff != ScoreZero; }

	bool timeToPick(const int depth) const { return depth == 1 + m_level; }

	Move pickMove(Rucksack* s) {
		// level については未対応。max_random_score_diff についてのみ対応する。
		if (m_maxRandomScoreDiff != ScoreZero) {
			size_t i = 1;
			for (; i < s->m_pvSize; ++i) {
				if (m_maxRandomScoreDiff < s->m_rootMoves[0].m_score_ - s->m_rootMoves[i].m_score_)
					break;
			}
			// 0 から i-1 までの間でランダムに選ぶ。
			std::uniform_int_distribution<size_t> dist(0, i - 1);
			m_best = s->m_rootMoves[dist(g_randomTimeSeed)].m_pv_[0];
			return m_best;
		}
		m_best = s->m_rootMoves[0].m_pv_[0];
		return m_best;
	}

	// レベル☆？
	int m_level;

	// ランダムな評価値の差分の最大値か☆？？
	ScoreIndex m_maxRandomScoreDiff;

	// ベストムーブか☆？
	Move m_best;
};
