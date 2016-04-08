#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n240_tt______/n240_300_tt.hpp"
#include "../n260_evaluate/n260_700_evaluate.hpp"
#include "n450_100_nodeType.hpp"
#include "n450_150_limitsType.hpp"
#include "n450_200_splitPoint.hpp"
#include "n450_250_thread.hpp"
#include "n450_300_mainThread.hpp"
#include "n450_350_timeThread.hpp"

const int g_MaxThreads = 64;

struct Thread;
class SearchStack;
class MovePicker;


class ThreadPool : public std::vector<Thread*> {
public:
	void Init(Searcher* s);
	void Exit();

	MainThread* GetMainThread() { return static_cast<MainThread*>((*this)[0]); }
	Depth GetMinSplitDepth() const { return m_minimumSplitDepth_; }
	TimerThread* GetTimerThread() { return m_timer_; }
	void WakeUp(Searcher* s);
	void Sleep();
	void ReadUSIOptions(Searcher* s);
	Thread* GetAvailableSlave(Thread* master) const;
	void SetTimer(const int msec);
	void WaitForThinkFinished();
	void StartThinking(const Position& pos, const LimitsType& limits,
					   const std::vector<Move>& searchMoves);

	bool m_isSleepWhileIdle_;
	size_t m_maxThreadsPerSplitPoint_;
	Mutex m_mutex_;
	ConditionVariable m_sleepCond_;

private:
	TimerThread* m_timer_;
	Depth m_minimumSplitDepth_;
};

