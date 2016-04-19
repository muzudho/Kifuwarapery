#include "../../header/n886_repeType/n886_500_rtArray.hpp"


#include "../../header/n886_repeType/n886_070_rtAbstract.hpp"
#include "../../header/n886_repeType/n886_100_rtNot.hpp"
#include "../../header/n886_repeType/n886_110_rtDraw.hpp"
#include "../../header/n886_repeType/n886_120_rtWin.hpp"
#include "../../header/n886_repeType/n886_130_rtLose.hpp"
#include "../../header/n886_repeType/n886_140_rtSuperior.hpp"
#include "../../header/n886_repeType/n886_150_rtInferior.hpp"
#include "../../header/n886_repeType/n886_500_rtArray.hpp"


const RepetitionTypeNot RepetitionTypeArray::m_repetitionTypeNot;
const RepetitionTypeDraw RepetitionTypeArray::m_repetitionTypeDraw;
const RepetitionTypeWin RepetitionTypeArray::m_repetitionTypeWin;
const RepetitionTypeLose RepetitionTypeArray::m_repetitionTypeLose;
const RepetitionTypeSuperior RepetitionTypeArray::m_repetitionTypeSuperior;
const RepetitionTypeInferior RepetitionTypeArray::m_repetitionTypeInferior;

const RepetitionTypeAbstract* RepetitionTypeArray::m_repetitionTypeArray[6] = {
	&RepetitionTypeArray::m_repetitionTypeNot,
	&RepetitionTypeArray::m_repetitionTypeDraw,
	&RepetitionTypeArray::m_repetitionTypeWin,
	&RepetitionTypeArray::m_repetitionTypeLose,
	&RepetitionTypeArray::m_repetitionTypeSuperior,
	&RepetitionTypeArray::m_repetitionTypeInferior,
};


RepetitionTypeArray g_repetitionTypeArray;
