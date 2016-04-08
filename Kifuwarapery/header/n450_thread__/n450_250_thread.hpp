#pragma once

#include <thread>	//std::thread
#include "../n080_common__/n080_100_common.hpp"
#include "../n119_score___/n119_090_score.hpp"
#include "../n223_move____/n223_100_move.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "n450_100_nodeType.hpp"
#include "n450_200_splitPoint.hpp"

class MovePicker;	//#include "../n280_move____/n280_150_movePicker.hpp"
class SearchStack;	//#include "../n320_searcher/n320_150_search.hpp"
class Searcher;
//struct SplitPoint;
class Position;


const int g_MaxSplitPointsPerThread = 8;




struct Thread {
	explicit Thread(Searcher* s);
	virtual ~Thread() {};

	virtual void IdleLoop();
	void NotifyOne();
	bool CutoffOccurred() const;
	bool IsAvailableTo(Thread* master) const;
	void WaitFor(volatile const bool& b);

	template <bool Fake>
	void Split(
		Position& pos,
		SearchStack* ss,
		const Score alpha,
		const Score beta,
		Score& bestScore,
		Move& bestMove,
		const Depth depth,
		const Move threatMove,
		const int moveCount,
		MovePicker& mp,
		const NodeType nodeType,
		const bool cutNode
	);

	SplitPoint m_SplitPoints[g_MaxSplitPointsPerThread];
	Position* m_activePosition;
	int m_idx;
	int m_maxPly;
	Mutex m_sleepLock;
	ConditionVariable m_sleepCond;
	std::thread m_handle;
	SplitPoint* volatile m_activeSplitPoint;
	volatile int m_splitPointsSize;
	volatile bool m_searching;
	volatile bool m_exit;
	Searcher* m_pSearcher;
};

