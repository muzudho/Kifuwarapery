#include "../../header/n165_movStack/n165_300_moveType.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n407_moveGen_/n407_900_moveList.hpp"
#include "../../header/n560_timeMng_/n560_100_limitsOfThinking.hpp"
#include "../../header/n640_searcher/n640_450_rootMove.hpp"
#include "../../header/n760_thread__/n760_250_thread.hpp"
#include "../../header/n760_thread__/n760_400_herosPub.hpp"
#include "../../header/n885_searcher/n885_500_rucksack.hpp"



//namespace {
template <typename T> T* newThread(Rucksack* s) {
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

void HerosPub::Init(Rucksack* s) {
	m_isSleepWhileIdle_ = true;
#if defined LEARN
#else
	m_pWarrior_ = newThread<Warrior>(s);
#endif
	push_back(newThread<MainThread>(s));
	ReadUSIOptions(s);
}

void HerosPub::Exit() {
#if defined LEARN
#else
	// checkTime() がデータにアクセスしないよう、先に timer_ を delete
	deleteThread(m_pWarrior_);
#endif

	for (auto elem : *this) {
		deleteThread(elem);
	}
}

void HerosPub::ReadUSIOptions(Rucksack* searcher) {

	this->m_maxThreadsPerSplitedNode_ = searcher->m_engineOptions["Max_Threads_per_Split_Point"];

	// スレッドの個数（１以上）
	const size_t numberOfThreads   = searcher->m_engineOptions["Threads"];

	this->m_minimumSplitDepth_ = (
		numberOfThreads < 6 ?
			4 :
			(numberOfThreads < 8 ?
				5 :
				7
			)
	) * OnePly;
	assert(0 < numberOfThreads);

	while (size() < numberOfThreads) {
		push_back(newThread<Thread>(searcher));
	}

	while (numberOfThreads < size()) {
		deleteThread(back());
		pop_back();
	}
}

Thread* HerosPub::GetAvailableSlave(Thread* master) const {
	for (auto elem : *this) {
		if (elem->IsAvailableTo(master)) {
			return elem;
		}
	}
	return nullptr;
}

void HerosPub::SetCurrWorrior(const int msec) {
	m_pWarrior_->m_maxPly = msec;
	m_pWarrior_->NotifyOne(); // Wake up and restart the timer
}

void HerosPub::WaitForThinkFinished() {
	MainThread* t = GetMainThread();
	std::unique_lock<Mutex> lock(t->m_sleepLock);
	m_sleepCond_.wait(lock, [&] { return !(t->m_isThinking); });
}

void HerosPub::StartThinking(
	const Position& position,
	const LimitsOfThinking& limits,
	const std::vector<Move>& searchMoves
)
{
#if defined LEARN
#else
	WaitForThinkFinished();
#endif
	position.GetSearcher()->m_stopwatchForSearch.Restart();

	position.GetSearcher()->m_signals.m_stopOnPonderHit = position.GetSearcher()->m_signals.m_firstRootMove = false;
	position.GetSearcher()->m_signals.m_stop = position.GetSearcher()->m_signals.m_failedLowAtRoot = false;

	position.GetSearcher()->m_rootPosition = position;
	position.GetSearcher()->m_limits = limits;
	position.GetSearcher()->m_rootMoves.clear();

#if defined LEARN
	// searchMoves を直接使う。
	GetPos.GetSearcher()->m_rootMoves.push_back(RootMove(position.GetSearcher()->m_ourMoves[0]));
#else
	const MoveType MT = N08_Legal;
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
