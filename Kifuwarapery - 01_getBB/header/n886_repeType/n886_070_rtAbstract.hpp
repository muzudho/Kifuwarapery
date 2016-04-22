#pragma once


#include "../n119_score___/n119_090_scoreIndex.hpp"

#include "../n223_move____/n223_500_flashlight.hpp"// &参照は使えない。*参照は使える。
#include "../n885_searcher/n885_500_rucksack.hpp"


class RepetitionTypeAbstract {
public:

	virtual void CheckStopAndMaxPly(
		bool& isReturn, ScoreIndex& resultScore, const Rucksack* pSearcher, Flashlight* pFlashlightBox) const = 0;

};

