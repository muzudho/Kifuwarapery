#pragma once

#include "../n260_evaluate/n260_700_evaluate.hpp"

struct LimitsType;

class TimeManager {
public:

	// 初期化？
	void Init(LimitsType& limits, const Ply currentPly, const Color us, Searcher* s);

	// 本譜の情勢不安定さ☆？
	void PvInstability(const int currChanges, const int prevChanges);

	// 利用可能時間？
	int AvailableTime() const { return m_optimumSearchTime_ + m_unstablePVExtraTime_; }

	// 最大時間？
	int MaximumTime() const { return m_maximumSearchTime_; }

private:

	// 最適な検索時間？
	int m_optimumSearchTime_;

	// 最大検索時間？
	int m_maximumSearchTime_;

	// 不安定な本譜のその他の時間？
	int m_unstablePVExtraTime_;
};

