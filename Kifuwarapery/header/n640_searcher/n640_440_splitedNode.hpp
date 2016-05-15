#pragma once


#include "../n080_common__/n080_085_mutex.hpp"	//Mutex
//#include "../n080_common__/n080_090_syncCout.hpp"
#include "../n080_common__/n080_100_common.hpp"
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"


class Military;
class SwordAbstract;


// 元の名前は ＳｐｌｉｔＰｏｉｎｔ☆
// ツリーノードの、分岐しているところだろうか☆（＾ｑ＾）？
struct SplitedNode {

	// 局面か☆
	const Position*		m_position;

	// サーチ・スタック☆
	const Flashlight*	m_pFlashlightBox;

	Military*				m_masterThread;

	Depth				m_depth;

	ScoreIndex				m_beta;

	// スプリット・ポイントは　ノード・タイプを持っている☆？（＾ｑ＾）
	const SwordAbstract*		m_pSword01;

	Move				m_threatMove;

	bool				m_cutNode;

	NextmoveEvent*		m_pVideodeck;

	SplitedNode*		m_pParentSplitedNode;

	Mutex				m_mutex;

	volatile u64		m_slavesMask;

	volatile s64		m_nodes;

	volatile ScoreIndex		m_alpha;

	volatile ScoreIndex		m_bestScore;

	volatile Move		m_bestMove;

	volatile int		m_moveCount;

	volatile bool		m_cutoff;
};
