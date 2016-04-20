#pragma once


#include "../n119_score___/n119_090_score.hpp"
#include "n886_070_rtAbstract.hpp" // &参照は使えない。*参照は使える。
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n885_searcher/n885_500_searcher.hpp"


class RepetitionTypeInferior : public RepetitionTypeAbstract {
public:

	void CheckStopAndMaxPly(
		bool& isReturn, Score& resultScore, const Searcher* pSearcher, Flashlight* pFlashlightBox
	) const {
		if (pFlashlightBox->m_ply != 2) {
			isReturn = true;
			resultScore = ScoreMatedInMaxPly;
			return;
		}
	}
};

