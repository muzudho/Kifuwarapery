#pragma once


#include <iostream>
#include "../n080_common__/n080_090_syncCout.hpp"
#include "../n080_common__/n080_100_common.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "n885_040_rucksack.hpp"


class HitchhikerNyugyoku {
public:
	static inline void Travel_885_480(Rucksack& rucksack, const Position& pos)
	{
		SYNCCOUT << "info nodes " << pos.GetNodesSearched()
			<< " time " << rucksack.m_stopwatch.GetElapsed() << SYNCENDL;

		if (!rucksack.m_signals.m_stop && (rucksack.m_limits.m_ponder || rucksack.m_limits.m_infinite)) {
			rucksack.m_signals.m_stopOnPonderHit = true;
			pos.GetThisThread()->WaitFor(rucksack.m_signals.m_stop);
		}

		SYNCCOUT << "bestmove win" << SYNCENDL;
	}
};