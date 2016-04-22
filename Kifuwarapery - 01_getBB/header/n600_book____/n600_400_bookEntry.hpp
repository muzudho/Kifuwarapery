#pragma once


#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n119_score___/n119_090_scoreIndex.hpp"


struct BookEntry {
	Key		m_key;
	u16		m_fromToPro;
	u16		m_count;
	ScoreIndex	m_score;
};
