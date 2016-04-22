#include <mutex>
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n223_move____/n223_040_nodeType.hpp"
#include "../../header/n223_move____/n223_200_depth.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../../header/n640_searcher/n640_440_splitedNode.hpp"
#include "../../header/n760_thread__/n760_250_military.hpp"
#include "../../header/n885_searcher/n885_500_rucksack.hpp"


Military::Military(Rucksack* searcher) /*: ＳｐｌｉｔＰｏｉｎｔｓ()*/ {
	m_pRucksack = searcher;
	m_exit = false;
	m_searching = false;
	m_splitedNodesSize = 0;
	m_maxPly = 0;
	m_activeSplitedNode = nullptr;
	m_activePosition = nullptr;
	m_idx = searcher->m_ownerHerosPub.size();
}

void Military::NotifyOne() {
	std::unique_lock<Mutex> lock(m_sleepLock);
	m_sleepCond.notify_one();
}

bool Military::CutoffOccurred() const {
	for (SplitedNode* sp = m_activeSplitedNode; sp != nullptr; sp = sp->m_pParentSplitedNode) {
		if (sp->m_cutoff) {
			return true;
		}
	}
	return false;
}

// master と同じ thread であるかを判定
bool Military::IsAvailableTo(Military* master) const {
	if (m_searching) {
		return false;
	}

	// ローカルコピーし、途中で値が変わらないようにする。
	const int spCount = m_splitedNodesSize;
	return !spCount || (m_SplitedNodes[spCount - 1].m_slavesMask & (UINT64_C(1) << master->m_idx));
}

void Military::WaitFor(volatile const bool& b) {
	std::unique_lock<Mutex> lock(m_sleepLock);
	m_sleepCond.wait(lock, [&] { return b; });
}


template <bool Fake>
void Military::ForkNewFighter(
	Position& pos,
	Flashlight* pFlashlightBox,
	const ScoreIndex alpha,
	const ScoreIndex beta,
	ScoreIndex& bestScore,
	Move& bestMove,
	const Depth depth,
	const Move threatMove,
	const int moveCount,
	NextmoveEvent& mp,
	const NodeType nodeType,
	const bool cutNode
	)
{
	assert(pos.IsOK());
	assert(bestScore <= alpha && alpha < beta && beta <= ScoreInfinite);
	assert(-ScoreInfinite < bestScore);
	assert(m_pRucksack->m_ownerHerosPub.GetMinSplitDepth() <= depth);

	assert(m_searching);
	assert(m_splitedNodesSize < g_MaxSplitedNodesPerThread);

	SplitedNode& sp = m_SplitedNodes[m_splitedNodesSize];

	sp.m_masterThread = this;
	sp.m_pParentSplitedNode = m_activeSplitedNode;
	sp.m_slavesMask = UINT64_C(1) << m_idx;
	sp.m_depth = depth;
	sp.m_bestMove = bestMove;
	sp.m_threatMove = threatMove;
	sp.m_alpha = alpha;
	sp.m_beta = beta;
	sp.m_nodeType = nodeType;
	sp.m_cutNode = cutNode;
	sp.m_bestScore = bestScore;
	sp.m_pNextmoveEvent = &mp;
	sp.m_moveCount = moveCount;
	sp.m_position = &pos;
	sp.m_nodes = 0;
	sp.m_cutoff = false;
	sp.m_pFlashlightBox = pFlashlightBox;

	m_pRucksack->m_ownerHerosPub.m_mutex_.lock();
	sp.m_mutex.lock();

	++m_splitedNodesSize;
	m_activeSplitedNode = &sp;
	m_activePosition = nullptr;

	// thisThread が常に含まれるので 1
	size_t slavesCount = 1;
	Military* slave;

	while ((slave = m_pRucksack->m_ownerHerosPub.GetAvailableSlave(this)) != nullptr
		&& ++slavesCount <= m_pRucksack->m_ownerHerosPub.m_maxThreadsPerSplitedNode_ && !Fake)
	{
		sp.m_slavesMask |= UINT64_C(1) << slave->m_idx;
		slave->m_activeSplitedNode = &sp;
		slave->m_searching = true;
		slave->NotifyOne();
	}

	if (1 < slavesCount || Fake) {
		sp.m_mutex.unlock();
		m_pRucksack->m_ownerHerosPub.m_mutex_.unlock();
		Military::IdleLoop();
		assert(!m_searching);
		assert(!m_activePosition);
		m_pRucksack->m_ownerHerosPub.m_mutex_.lock();
		sp.m_mutex.lock();
	}

	m_searching = true;
	--m_splitedNodesSize;
	m_activeSplitedNode = sp.m_pParentSplitedNode;
	m_activePosition = &pos;
	pos.SetNodesSearched(pos.GetNodesSearched() + sp.m_nodes);
	bestMove = sp.m_bestMove;
	bestScore = sp.m_bestScore;

	m_pRucksack->m_ownerHerosPub.m_mutex_.unlock();
	sp.m_mutex.unlock();
}

template void Military::ForkNewFighter<true >(
	Position& pos, Flashlight* ss, const ScoreIndex alpha, const ScoreIndex beta, ScoreIndex& bestScore,
	Move& bestMove, const Depth depth, const Move threatMove, const int moveCount,
	NextmoveEvent& mp, const NodeType nodeType, const bool cutNode
);

template void Military::ForkNewFighter<false>(
	Position& pos, Flashlight* ss, const ScoreIndex alpha, const ScoreIndex beta, ScoreIndex& bestScore,
	Move& bestMove, const Depth depth, const Move threatMove, const int moveCount,
	NextmoveEvent& mp, const NodeType nodeType, const bool cutNode
);
