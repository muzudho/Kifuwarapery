#pragma once

#include "../n260_evaluate/n260_700_evaluate.hpp"

struct LimitsType;

class TimeManager {
public:

	// 初期化？
	void init(LimitsType& limits, const Ply currentPly, const Color us, Searcher* s);

	// 本譜の情勢不安定さ☆？
	void pvInstability(const int currChanges, const int prevChanges);

	// 利用可能時間？
	int availableTime() const { return optimumSearchTime_ + unstablePVExtraTime_; }

	// 最大時間？
	int maximumTime() const { return maximumSearchTime_; }

private:

	// 最適な検索時間？
	int optimumSearchTime_;

	// 最大検索時間？
	int maximumSearchTime_;

	// 不安定な本譜のその他の時間？
	int unstablePVExtraTime_;
};

