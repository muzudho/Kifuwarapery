#include "../../header/n276_genMove_/n276_250_makePromoteMove.hpp"
#include "../../header/n320_searcher/n320_550_search.hpp"
#include "../../header/n450_thread__/n450_250_thread.hpp"
#include "../../header/n450_thread__/n450_400_threadPool.hpp"




//namespace {
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
//}

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

	for (auto elem : *this) {
		deleteThread(elem);
	}
}

void ThreadPool::ReadUSIOptions(Searcher* s) {
	m_maxThreadsPerSplitPoint_ = s->m_engineOptions["Max_Threads_per_Split_Point"];
	const size_t requested   = s->m_engineOptions["Threads"];
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

void ThreadPool::StartThinking(
	const Position& pos,
	const LimitsType& limits,
	const std::vector<Move>& searchMoves
)
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
	this->GetMainThread()->m_isThinking = true;
	this->GetMainThread()->NotifyOne();
#endif
}
