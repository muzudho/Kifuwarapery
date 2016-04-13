#include "../../header/n165_movStack/n165_300_moveType.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n223_move____/n223_050_limitsType.hpp"
#include "../../header/n276_genMove_/n276_150_moveList.hpp"
#include "../../header/n320_searcher/n320_137_rootMove.hpp"
#include "../../header/n450_thread__/n450_250_thread.hpp"
#include "../../header/n450_thread__/n450_400_threadPool.hpp"
#include "../../header/n900_main____/n900_200_searcher.hpp"



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

void ThreadPool::ReadUSIOptions(Searcher* searcher) {
	m_maxThreadsPerSplitPoint_ = searcher->m_engineOptions["Max_Threads_per_Split_Point"];
	const size_t requested   = searcher->m_engineOptions["Threads"];
	m_minimumSplitDepth_ = (requested < 6 ? 4 : (requested < 8 ? 5 : 7)) * OnePly;

	assert(0 < requested);

	while (size() < requested) {
		push_back(newThread<Thread>(searcher));
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
	const Position& position,
	const LimitsType& limits,
	const std::vector<Move>& searchMoves
)
{
#if defined LEARN
#else
	WaitForThinkFinished();
#endif
	position.GetSearcher()->m_searchTimer.Restart();

	position.GetSearcher()->m_signals.m_stopOnPonderHit = position.GetSearcher()->m_signals.m_firstRootMove = false;
	position.GetSearcher()->m_signals.m_stop = position.GetSearcher()->m_signals.m_failedLowAtRoot = false;

	position.GetSearcher()->m_rootPosition = position;
	position.GetSearcher()->m_limits = limits;
	position.GetSearcher()->m_rootMoves.clear();

#if defined LEARN
	// searchMoves を直接使う。
	GetPos.GetSearcher()->m_rootMoves.push_back(RootMove(m_searchMoves[0]));
#else
	const MoveType MT = Legal;
	for (MoveList<MT> ml(position); !ml.IsEnd(); ++ml) {
		if (searchMoves.empty()
			|| std::find(searchMoves.begin(), searchMoves.end(), ml.GetMove()) != searchMoves.end())
		{
			position.GetSearcher()->m_rootMoves.push_back(RootMove(ml.GetMove()));
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
