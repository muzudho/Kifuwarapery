#pragma once


#include "n885_310_hitchhikerQsearchAbstract.hpp"
#include "n885_320_hitchhikerQsearchPv.hpp"
#include "n885_330_hitchhikerQsearchNonPv.hpp"


class HitchhikerQsearchPrograms {
public:

	static const HitchhikerQsearchPv m_hitchhikerQsearchPv;
	static const HitchhikerQsearchNonPv m_hitchhikerQsearchNonPv;

	static const HitchhikerQsearchAbstract* m_pHitchhikerQsearchPrograms[3];

};


extern HitchhikerQsearchPrograms g_hitchhikerQsearchPrograms;
