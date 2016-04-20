#pragma once


#include "../n119_score___/n119_090_score.hpp"

#include "../n223_move____/n223_500_flashlight.hpp"// &参照は使えない。*参照は使える。
#include "../n885_searcher/n885_500_searcher.hpp"


class RepetitionTypeAbstract {
public:

	virtual void CheckStopAndMaxPly(
		bool& isReturn, Score& resultScore, const Searcher* pSearcher, Flashlight* pFlashlightBox) const = 0;

};

