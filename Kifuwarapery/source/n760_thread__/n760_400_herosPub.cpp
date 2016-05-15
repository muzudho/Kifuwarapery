#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n165_movStack/n165_300_moveType.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n300_moveGen_/n300_700_moveGen_/n300_700_900_moveList.hpp"
#include "../../header/n560_timeMng_/n560_100_limitsOfThinking.hpp"
#include "../../header/n640_searcher/n640_450_rootMove.hpp"
#include "../../header/n760_thread__/n760_250_military.hpp"
#include "../../header/n760_thread__/n760_400_herosPub.hpp"
#include "../../header/n885_searcher/n885_040_rucksack.hpp"



//namespace {
template <typename T> T* newThread(Rucksack* s) {
	T* th = new T(s);
	th->m_handle = std::thread(&Military::IdleLoop, th); // move constructor
	return th;
}
void deleteThread(Military* th) {
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
	push_back(newThread<Captain>(s));
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
		push_back(newThread<Military>(searcher));
	}

	while (numberOfThreads < size()) {
		deleteThread(back());
		pop_back();
	}
}

Military* HerosPub::GetAvailableSlave(Military* master) const {
	for (auto elem : *this) {
		if (elem->IsAvailableTo(master)) {
			return elem;
		}
	}
	return nullptr;
}

// 元の引数名はｍｓｅｃ☆
void HerosPub::SetCurrWorrior(const int maxPly) {
	m_pWarrior_->m_maxPly = maxPly;
	m_pWarrior_->NotifyOne(); // Wake up and restart the timer
}

void HerosPub::WaitForThinkFinished() {
	Captain* t = GetFirstCaptain();
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
	position.GetRucksack()->m_stopwatch.Restart();

	position.GetRucksack()->m_signals.m_stopOnPonderHit = position.GetRucksack()->m_signals.m_firstRootMove = false;
	position.GetRucksack()->m_signals.m_stop = position.GetRucksack()->m_signals.m_failedLowAtRoot = false;

	position.GetRucksack()->m_rootPosition = position;
	position.GetRucksack()->m_limits = limits;
	position.GetRucksack()->m_rootMoves.clear();

#if defined LEARN
	// searchMoves を直接使う。
	position.GetRucksack()->m_rootMoves.push_back(RootMove(position.GetRucksack()->m_ourMoves[0]));
#else
	const MoveType MT = N08_Legal;
	for (MoveList<MT> ml(position); !ml.IsEnd(); ++ml) {
		if (searchMoves.empty()
			|| std::find(searchMoves.begin(), searchMoves.end(), ml.GetMove()) != searchMoves.end())
		{
			position.GetRucksack()->m_rootMoves.push_back(RootMove(ml.GetMove()));
		}
	}
#endif

#if defined LEARN
	// 浅い探索なので、thread 生成、破棄のコストが高い。余分な thread を生成せずに直接探索を呼び出す。
	Hitchhiker::Think(*position.GetRucksack());
#else
	this->GetFirstCaptain()->m_isThinking = true;
	this->GetFirstCaptain()->NotifyOne();
#endif
}
