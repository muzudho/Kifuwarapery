#pragma once

#include <thread>	//std::thread
#include "../n080_common__/n080_100_common.hpp"
#include "../n119_score___/n119_090_score.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp" // 持ち合い

class Rucksack;


const int g_MaxSplitedNodesPerThread = 8;



// 元の名前：Ｔｈｒｅａｄ
// ゲームツリーを戦線拡大していくぜ☆！（＾ｑ＾）
struct Military {

	explicit Military(Rucksack* s);

	virtual ~Military() {};

	virtual void IdleLoop();

	void NotifyOne();

	bool CutoffOccurred() const;

	bool IsAvailableTo(Military* master) const;

	void WaitFor(volatile const bool& b);

	// 元の名前： Ｓｐｌｉｔ
	// 探索を分けるのだろうか☆？兵士をどんどん増やそうぜ☆（＾ｑ＾）
	template <bool Fake>
	void ForkNewFighter(
		Position& pos,
		Flashlight* pFlashlightBox,
		const Score alpha,
		const Score beta,
		Score& bestScore,
		Move& bestMove,
		const Depth depth,
		const Move threatMove,
		const int moveCount,
		NextmoveEvent& mp,
		const NodeType nodeType,
		const bool cutNode
	);

	SplitedNode m_SplitedNodes[g_MaxSplitedNodesPerThread];

	Position* m_activePosition;

	int m_idx;

	int m_maxPly;

	Mutex m_sleepLock;

	ConditionVariable m_sleepCond;

	std::thread m_handle;

	SplitedNode* volatile m_activeSplitedNode;

	volatile int m_splitedNodesSize;

	volatile bool m_searching;

	volatile bool m_exit;

	Rucksack* m_pSearcher;
};

