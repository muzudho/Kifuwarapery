#include "../../header/n450_thread__/n450_250_thread.hpp"


#include "../../header/n900_main____/n900_200_searcher.hpp"		//TODO:1 これを外すのがむずかしい。
//class Searcher;


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


template <bool Fake>
void Thread::Split(
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
	)
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
	sp.m_position = &pos;
	sp.m_nodes = 0;
	sp.m_cutoff = false;
	sp.m_searchStack = ss;

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
