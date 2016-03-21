#pragma once

#include "stdafx.h"
#include "n240_250_evaluate.hpp"

struct LimitsType;

class TimeManager {
public:
	void init(LimitsType& limits, const Ply currentPly, const Color us, Searcher* s);
	void pvInstability(const int currChanges, const int prevChanges);
	int availableTime() const { return optimumSearchTime_ + unstablePVExtraTime_; }
	int maximumTime() const { return maximumSearchTime_; }

private:
	int optimumSearchTime_;
	int maximumSearchTime_;
	int unstablePVExtraTime_;
};

