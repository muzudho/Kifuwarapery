#pragma once


#include "n886_070_rtAbstract.hpp"
#include "n886_100_rtNot.hpp"
#include "n886_110_rtDraw.hpp"
#include "n886_120_rtWin.hpp"
#include "n886_130_rtLose.hpp"
#include "n886_140_rtSuperior.hpp"
#include "n886_150_rtInferior.hpp"


class RepetitionTypeArray {
public:

	static const RepetitionTypeNot m_repetitionTypeNot;
	static const RepetitionTypeDraw m_repetitionTypeDraw;
	static const RepetitionTypeWin m_repetitionTypeWin;
	static const RepetitionTypeLose m_repetitionTypeLose;
	static const RepetitionTypeSuperior m_repetitionTypeSuperior;
	static const RepetitionTypeInferior m_repetitionTypeInferior;

	static const RepetitionTypeAbstract* m_repetitionTypeArray[6];

};


extern RepetitionTypeArray g_repetitionTypeArray;
