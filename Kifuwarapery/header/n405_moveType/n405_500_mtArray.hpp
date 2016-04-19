#pragma once


#include "../n405_moveType/n405_070_mtAbstract.hpp"
#include "../n405_moveType/n405_100_mtCapture.hpp"
#include "../n405_moveType/n405_110_mtNonCapture.hpp"
#include "../n405_moveType/n405_120_mtDrop.hpp"
#include "../n405_moveType/n405_130_mtCapturePlusPro.hpp"
#include "../n405_moveType/n405_140_mtNonCaptureMinusPro.hpp"
#include "../n405_moveType/n405_150_mtRecapture.hpp"
#include "../n405_moveType/n405_160_mtEvasion.hpp"
#include "../n405_moveType/n405_170_mtNonEvasion.hpp"
#include "../n405_moveType/n405_180_mtLegal.hpp"
#include "../n405_moveType/n405_190_mtLegalAll.hpp"
#include "../n405_moveType/n405_200_mtMoveTypeNone.hpp"


class MoveTypeArray {
public:
	//(^q^)const型の配列を作るには、static でやるんだぜ☆
	static const MoveTypeCapture m_moveTypeCapture;
	static const MoveTypeNonCapture m_moveTypeNonCapture;
	static const MoveTypeDrop m_moveTypeDrop;
	static const MoveTypeCapturePlusPro m_moveTypeCapturePlusPro;
	static const MoveTypeNonCaptureMinusPro m_moveTypeNonCaptureMinusPro;
	static const MoveTypeRecapture m_moveTypeRecapture;
	static const MoveTypeEvasion m_moveTypeEvasion;
	static const MoveTypeNonEvasion m_moveTypeNonEvasion;
	static const MoveTypeLegal m_moveTypeLegal;
	static const MoveTypeLegalAll m_moveTypeLegalAll;
	static const MoveTypeNone m_moveTypeNone;

	static const MoveTypeAbstract* m_moveTypeArray[11];

};


extern MoveTypeArray g_moveTypeArray;
