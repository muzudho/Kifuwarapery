#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_050_limitsType.hpp"
#include "../n223_move____/n223_500_searchStack.hpp"
#include "../n440_movStack/n440_500_movePicker.hpp"
#include "../n480_tt______/n480_300_tt.hpp"
#include "../n520_evaluate/n520_700_evaluation09.hpp"
#include "../n640_searcher/n640_440_splitPoint.hpp"
#include "n760_250_thread.hpp"
#include "n760_300_mainThread.hpp"
#include "n760_350_timerThread.hpp"

const int g_MaxThreads = 64;

class Searcher;


class ThreadPool : public std::vector<Thread*> {
public:
	// 初期化？
	void Init(Searcher* s);

	// 終了？
	void Exit();

	// メインスレッド？
	MainThread* GetMainThread() { return static_cast<MainThread*>((*this)[0]); }

	// 最小の深さ？
	Depth GetMinSplitDepth() const { return m_minimumSplitDepth_; }

	// タイマースレッド？
	TimerThread* GetTimerThread() { return m_timer_; }

	// 起きろ？
	void WakeUp(Searcher* s);

	// 寝ろ？
	void Sleep();

	// USIオプションを読め？
	void ReadUSIOptions(Searcher* s);

	// スレーブ？
	Thread* GetAvailableSlave(Thread* master) const;

	// タイマーをセット？
	void SetTimer(const int msec);

	// 考えが終わるまで待て？
	void WaitForThinkFinished();

	// 考えを始めろ？
	void StartThinking(const Position& pos, const LimitsType& limits,
					   const std::vector<Move>& searchMoves);

public:
	// 寝ているか？
	bool m_isSleepWhileIdle_;

	// マックス・スレッド？
	size_t m_maxThreadsPerSplitPoint_;

	// ミューテックス？
	Mutex m_mutex_;

	// 寝ている条件？
	ConditionVariable m_sleepCond_;

private:

	TimerThread* m_timer_;

	Depth m_minimumSplitDepth_;
};

