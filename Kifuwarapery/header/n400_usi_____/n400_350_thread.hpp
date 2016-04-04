#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n240_tt______/n240_300_tt.hpp"
#include "../n260_evaluate/n260_700_evaluate.hpp"

const int g_MaxThreads = 64;
const int g_MaxSplitPointsPerThread = 8;

struct Thread;
struct SearchStack;
class MovePicker;

enum NodeType {
	Root, PV, NonPV, SplitPointRoot, SplitPointPV, SplitPointNonPV
};

// 時間や探索深さの制限を格納する為の構造体
struct LimitsType {

	LimitsType() { memset(this, 0, sizeof(LimitsType)); }

	bool IsUseTimeManagement() const { return !(m_depth | m_nodes | m_moveTime | static_cast<int>(m_infinite)); }

	int m_time[ColorNum];
	int m_increment[ColorNum];
	int m_movesToGo;
	Ply m_depth;
	u32 m_nodes;
	int m_moveTime;
	bool m_infinite;
	bool m_ponder;
};

struct SplitPoint {
	const Position*		m_pos;
	const SearchStack*	m_ss;
	Thread*				m_masterThread;
	Depth				m_depth;
	Score				m_beta;
	NodeType			m_nodeType;
	Move				m_threatMove;
	bool				m_cutNode;

	MovePicker*			m_pMovePicker;
	SplitPoint*			m_pParentSplitPoint;

	Mutex				m_mutex;
	volatile u64		m_slavesMask;
	volatile s64		m_nodes;
	volatile Score		m_alpha;
	volatile Score		m_bestScore;
	volatile Move		m_bestMove;
	volatile int		m_moveCount;
	volatile bool		m_cutoff;
};

struct Thread {
	explicit Thread(Searcher* s);
	virtual ~Thread() {};

	virtual void IdleLoop();
	void NotifyOne();
	bool CutoffOccurred() const;
	bool IsAvailableTo(Thread* master) const;
	void WaitFor(volatile const bool& b);

	template <bool Fake>
	void Split(Position& pos, SearchStack* ss, const Score alpha, const Score beta, Score& bestScore,
			   Move& bestMove, const Depth depth, const Move threatMove, const int moveCount,
			   MovePicker& mp, const NodeType nodeType, const bool cutNode);

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

struct MainThread : public Thread {
	explicit MainThread(Searcher* s) : Thread(s), m_isThinking(true) {}
	virtual void IdleLoop();
	volatile bool m_isThinking;
};

struct TimerThread : public Thread {
	explicit TimerThread(Searcher* s) : Thread(s), m_msec(0) {}
	virtual void IdleLoop();
	int m_msec;
};

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

