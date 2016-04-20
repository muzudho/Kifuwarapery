#pragma once


#include "../n119_score___/n119_090_score.hpp"
#include "n886_070_rtAbstract.hpp"

#include "../n223_move____/n223_500_flashlight.hpp"// &参照は使えない。*参照は使える。
#include "../n885_searcher/n885_500_rucksack.hpp"


class RepetitionTypeNot : public RepetitionTypeAbstract {
public:

	void CheckStopAndMaxPly(
		bool& isReturn, Score& resultScore, const Rucksack* pSearcher, Flashlight* pFlashlightBox
	) const {
		if (!pSearcher->m_signals.m_stop &&
			pFlashlightBox->m_ply <= g_maxPly
		) {
			return;
		}
	}

};

