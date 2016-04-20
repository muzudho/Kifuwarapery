#pragma once

#include "../n520_evaluate/n520_700_evaluation09.hpp"
#include "../n560_timeMng_/n560_100_limitsOfThinking.hpp"

class Searcher;


class TimeManager {
public:

	// 初期化？
	void Init(LimitsOfThinking& limits, const Ply currentPly, const Color us, Searcher* s);

	// 本譜の情勢不安定さ☆？
	void SetPvInstability(const int currChanges, const int prevChanges);

	// 利用可能時間？
	int GetAvailableTime() const { return m_optimumSearchTime_ + m_unstablePVExtraTime_; }

	// 最大時間？
	int GetMaximumTime() const { return m_maximumSearchTime_; }

private:

	// 理想的な思考時間？
	int m_optimumSearchTime_;

	// 長考を打ち切る時間？
	int m_maximumSearchTime_;

	// 不安定な本譜のその他の時間？
	int m_unstablePVExtraTime_;
};

