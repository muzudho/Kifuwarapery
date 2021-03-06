﻿#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_070_mtAbstract.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_100_mtCapture.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_110_mtNonCapture.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_120_mtDrop.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_130_mtCapturePlusPro.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_140_mtNonCaptureMinusPro.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_150_mtRecapture.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_160_mtEvasion.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_170_mtNonEvasion.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_180_mtLegal.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_190_mtLegalAll.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_200_mtMoveTypeNone.hpp"
#include "../../../header/n300_moveGen_/n300_600_moveType/n300_600_500_mtArray.hpp"


const MoveTypeCapture				MoveTypeArray::m_moveTypeCapture;
const MoveTypeNonCapture			MoveTypeArray::m_moveTypeNonCapture;
const MoveTypeDrop					MoveTypeArray::m_moveTypeDrop;
const MoveTypeCapturePlusPro		MoveTypeArray::m_moveTypeCapturePlusPro;
const MoveTypeNonCaptureMinusPro	MoveTypeArray::m_moveTypeNonCaptureMinusPro;
const MoveTypeRecapture				MoveTypeArray::m_moveTypeRecapture;
const MoveTypeEvasion				MoveTypeArray::m_moveTypeEvasion;
const MoveTypeNonEvasion			MoveTypeArray::m_moveTypeNonEvasion;
const MoveTypeLegal					MoveTypeArray::m_moveTypeLegal;
const MoveTypeLegalAll				MoveTypeArray::m_moveTypeLegalAll;
const MoveTypeNone					MoveTypeArray::m_moveTypeNone;

const MoveTypeAbstract*			MoveTypeArray::m_moveTypeArray[11] = {
	&MoveTypeArray::m_moveTypeCapture,
	&MoveTypeArray::m_moveTypeNonCapture,
	&MoveTypeArray::m_moveTypeDrop,
	&MoveTypeArray::m_moveTypeCapturePlusPro,
	&MoveTypeArray::m_moveTypeNonCaptureMinusPro,
	&MoveTypeArray::m_moveTypeRecapture,
	&MoveTypeArray::m_moveTypeEvasion,
	&MoveTypeArray::m_moveTypeNonEvasion,
	&MoveTypeArray::m_moveTypeLegal,
	&MoveTypeArray::m_moveTypeLegalAll,
	&MoveTypeArray::m_moveTypeNone
};


MoveTypeArray g_moveTypeArray;
