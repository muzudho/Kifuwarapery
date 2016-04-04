#include "../../header/n276_genMove_/n276_250_makePromoteMove.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"

namespace {
	template <typename T> T* newThread(Searcher* s) {
		T* th = new T(s);
		th->m_handle = std::thread(&Thread::IdleLoop, th); // move constructor
		return th;
	}
	void deleteThread(Thread* th) {
		th->m_exit = true;
		th->NotifyOne();
		th->m_handle.join(); // Wait for thread termination
		delete th;
	}
}

Thread::Thread(Searcher* s) /*: splitPoints()*/ {
	m_pSearcher = s;
	m_exit = false;
	m_searching = false;
	m_splitPointsSize = 0;
	m_maxPly = 0;
	m_activeSplitPoint = nullptr;
	m_activePosition = nullptr;
	m_idx = s->m_threads.size();
}

void TimerThread::IdleLoop() {
	while (!m_exit) {
		{
			std::unique_lock<Mutex> lock(m_sleepLock);
			if (!m_exit) {
				m_sleepCond.wait_for(lock, std::chrono::milliseconds(m_msec ? m_msec : INT_MAX));
			}
		}
		if (m_msec) {
			m_pSearcher->CheckTime();
		}
	}
}

void MainThread::IdleLoop() {
	while (true) {
		{
			std::unique_lock<Mutex> lock(m_sleepLock);
			m_isThinking = false;
			while (!m_isThinking && !m_exit) {
				// UI 関連だから要らないのかも。
				m_pSearcher->m_threads.m_sleepCond_.notify_one();
				m_sleepCond.wait(lock);
			}
		}

		if (m_exit) {
			return;
		}

		m_searching = true;
		m_pSearcher->Think();
		assert(m_searching);
		m_searching = false;
	}
}

void Thread::NotifyOne() {
	std::unique_lock<Mutex> lock(m_sleepLock);
	m_sleepCond.notify_one();
}

bool Thread::CutoffOccurred() const {
	for (SplitPoint* sp = m_activeSplitPoint; sp != nullptr; sp = sp->m_pParentSplitPoint) {
		if (sp->m_cutoff) {
			return true;
		}
	}
	return false;
}

// master と同じ thread であるかを判定
bool Thread::IsAvailableTo(Thread* master) const {
	if (m_searching) {
		return false;
	}

	// ローカルコピーし、途中で値が変わらないようにする。
	const int spCount = m_splitPointsSize;
	return !spCount || (m_SplitPoints[spCount - 1].m_slavesMask & (UINT64_C(1) << master->m_idx));
}

void Thread::WaitFor(volatile const bool& b) {
	std::unique_lock<Mutex> lock(m_sleepLock);
	m_sleepCond.wait(lock, [&] { return b; });
}

void ThreadPool::Init(Searcher* s) {
	m_isSleepWhileIdle_ = true;
#if defined LEARN
#else
	m_timer_ = newThread<TimerThread>(s);
#endif
	push_back(newThread<MainThread>(s));
	ReadUSIOptions(s);
}

void ThreadPool::Exit() {
#if defined LEARN
#else
	// checkTime() がデータにアクセスしないよう、先に timer_ を delete
	deleteThread(m_timer_);
#endif

	for (auto elem : *this)
		deleteThread(elem);
}

void ThreadPool::ReadUSIOptions(Searcher* s) {
	m_maxThreadsPerSplitPoint_ = s->m_options["Max_Threads_per_Split_Point"];
	const size_t requested   = s->m_options["Threads"];
	m_minimumSplitDepth_ = (requested < 6 ? 4 : (requested < 8 ? 5 : 7)) * OnePly;

	assert(0 < requested);

	while (size() < requested) {
		push_back(newThread<Thread>(s));
	}

	while (requested < size()) {
		deleteThread(back());
		pop_back();
	}
}

Thread* ThreadPool::GetAvailableSlave(Thread* master) const {
	for (auto elem : *this) {
		if (elem->IsAvailableTo(master)) {
			return elem;
		}
	}
	return nullptr;
}

void ThreadPool::SetTimer(const int msec) {
	m_timer_->m_maxPly = msec;
	m_timer_->NotifyOne(); // Wake up and restart the timer
}

void ThreadPool::WaitForThinkFinished() {
	MainThread* t = GetMainThread();
	std::unique_lock<Mutex> lock(t->m_sleepLock);
	m_sleepCond_.wait(lock, [&] { return !(t->m_isThinking); });
}

void ThreadPool::StartThinking(const Position& pos, const LimitsType& limits,
							   const std::vector<Move>& searchMoves)
{
#if defined LEARN
#else
	WaitForThinkFinished();
#endif
	pos.GetSearcher()->m_searchTimer.Restart();

	pos.GetSearcher()->m_signals.m_stopOnPonderHit = pos.GetSearcher()->m_signals.m_firstRootMove = false;
	pos.GetSearcher()->m_signals.m_stop = pos.GetSearcher()->m_signals.m_failedLowAtRoot = false;

	pos.GetSearcher()->m_rootPosition = pos;
	pos.GetSearcher()->m_limits = limits;
	pos.GetSearcher()->m_rootMoves.clear();

#if defined LEARN
	// searchMoves を直接使う。
	GetPos.GetSearcher()->m_rootMoves.push_back(RootMove(m_searchMoves[0]));
#else
	const MoveType MT = Legal;
	for (MoveList<MT> ml(pos); !ml.IsEnd(); ++ml) {
		if (searchMoves.empty()
			|| std::find(searchMoves.begin(), searchMoves.end(), ml.GetMove()) != searchMoves.end())
		{
			pos.GetSearcher()->m_rootMoves.push_back(RootMove(ml.GetMove()));
		}
	}
#endif

#if defined LEARN
	// 浅い探索なので、thread 生成、破棄のコストが高い。余分な thread を生成せずに直接探索を呼び出す。
	GetPos.GetSearcher()->Think();
#else
	GetMainThread()->m_isThinking = true;
	GetMainThread()->NotifyOne();
#endif
}

template <bool Fake>
void Thread::Split(Position& pos, SearchStack* ss, const Score alpha, const Score beta, Score& bestScore,
				   Move& bestMove, const Depth depth, const Move threatMove, const int moveCount,
				   MovePicker& mp, const NodeType nodeType, const bool cutNode)
{
	assert(pos.IsOK());
	assert(bestScore <= alpha && alpha < beta && beta <= ScoreInfinite);
	assert(-ScoreInfinite < bestScore);
	assert(m_pSearcher->m_threads.GetMinSplitDepth() <= depth);

	assert(m_searching);
	assert(m_splitPointsSize < g_MaxSplitPointsPerThread);

	SplitPoint& sp = m_SplitPoints[m_splitPointsSize];

	sp.m_masterThread = this;
	sp.m_pParentSplitPoint = m_activeSplitPoint;
	sp.m_slavesMask = UINT64_C(1) << m_idx;
	sp.m_depth = depth;
	sp.m_bestMove = bestMove;
	sp.m_threatMove = threatMove;
	sp.m_alpha = alpha;
	sp.m_beta = beta;
	sp.m_nodeType = nodeType;
	sp.m_cutNode = cutNode;
	sp.m_bestScore = bestScore;
	sp.m_pMovePicker = &mp;
	sp.m_moveCount = moveCount;
	sp.m_pos = &pos;
	sp.m_nodes = 0;
	sp.m_cutoff = false;
	sp.m_ss = ss;

	m_pSearcher->m_threads.m_mutex_.lock();
	sp.m_mutex.lock();

	++m_splitPointsSize;
	m_activeSplitPoint = &sp;
	m_activePosition = nullptr;

	// thisThread が常に含まれるので 1
	size_t slavesCount = 1;
	Thread* slave;

	while ((slave = m_pSearcher->m_threads.GetAvailableSlave(this)) != nullptr
		   && ++slavesCount <= m_pSearcher->m_threads.m_maxThreadsPerSplitPoint_ && !Fake)
	{
		sp.m_slavesMask |= UINT64_C(1) << slave->m_idx;
		slave->m_activeSplitPoint = &sp;
		slave->m_searching = true;
		slave->NotifyOne();
	}

	if (1 < slavesCount || Fake) {
		sp.m_mutex.unlock();
		m_pSearcher->m_threads.m_mutex_.unlock();
		Thread::IdleLoop();
		assert(!m_searching);
		assert(!m_activePosition);
		m_pSearcher->m_threads.m_mutex_.lock();
		sp.m_mutex.lock();
	}

	m_searching = true;
	--m_splitPointsSize;
	m_activeSplitPoint = sp.m_pParentSplitPoint;
	m_activePosition = &pos;
	pos.SetNodesSearched(pos.GetNodesSearched() + sp.m_nodes);
	bestMove = sp.m_bestMove;
	bestScore = sp.m_bestScore;

	m_pSearcher->m_threads.m_mutex_.unlock();
	sp.m_mutex.unlock();
}

template void Thread::Split<true >(Position& pos, SearchStack* ss, const Score alpha, const Score beta, Score& bestScore,
								   Move& bestMove, const Depth depth, const Move threatMove, const int moveCount,
								   MovePicker& mp, const NodeType nodeType, const bool cutNode);
template void Thread::Split<false>(Position& pos, SearchStack* ss, const Score alpha, const Score beta, Score& bestScore,
								   Move& bestMove, const Depth depth, const Move threatMove, const int moveCount,
								   MovePicker& mp, const NodeType nodeType, const bool cutNode);
