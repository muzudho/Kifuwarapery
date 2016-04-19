#pragma once


#include "../n119_score___/n119_090_score.hpp"

#include "../n223_move____/n223_500_searchStack.hpp"
//class SearchStack; // &参照は使えない。*参照は使える。

#include "../n885_searcher/n885_500_searcher.hpp"
//class Searcher;


class RepetitionTypeAbstract {
public:

	virtual void CheckStopAndMaxPly(
		bool& isReturn, Score& resultScore, const Searcher* pSearcher, SearchStack* pSs) const = 0;

};

