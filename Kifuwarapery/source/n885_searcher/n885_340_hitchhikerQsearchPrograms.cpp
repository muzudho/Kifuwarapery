#include "../../header/n885_searcher/n885_310_hitchhikerQsearchAbstract.hpp"
#include "../../header/n885_searcher/n885_320_hitchhikerQsearchPv.hpp"
#include "../../header/n885_searcher/n885_330_hitchhikerQsearchNonPv.hpp"
#include "../../header/n885_searcher/n885_340_hitchhikerQsearchPrograms.hpp"


const HitchhikerQsearchPv HitchhikerQsearchPrograms::m_hitchhikerQsearchPv;
const HitchhikerQsearchNonPv HitchhikerQsearchPrograms::m_hitchhikerQsearchNonPv;

const HitchhikerQsearchAbstract* HitchhikerQsearchPrograms::m_pHitchhikerQsearchPrograms[3] = {
	nullptr,
	&HitchhikerQsearchPrograms::m_hitchhikerQsearchPv,
	&HitchhikerQsearchPrograms::m_hitchhikerQsearchNonPv
};

