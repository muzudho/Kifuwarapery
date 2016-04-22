#pragma once


#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n119_score___/n119_100_utilScore.hpp"
#include "n886_070_rtAbstract.hpp"

#include "../n223_move____/n223_500_flashlight.hpp"// &�Q�Ƃ͎g���Ȃ��B*�Q�Ƃ͎g����B
#include "../n885_searcher/n885_500_rucksack.hpp"


class RepetitionTypeWin : public RepetitionTypeAbstract {
public:

	void CheckStopAndMaxPly(
		bool& isReturn, ScoreIndex& resultScore, const Rucksack* pSearcher, Flashlight* pFlashlightBox
	) const {
		isReturn = true;
		resultScore = UtilScore::MateIn(pFlashlightBox->m_ply);
		return;
	}
};
