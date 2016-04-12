#pragma once


#include "../n080_common__/n080_100_common.hpp"	//Mutexなど
#include "../n119_score___/n119_090_score.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_100_move.hpp"
#include "../n223_move____/n223_200_depth.hpp"
#include "../n223_move____/n223_500_searchStack.hpp"
#include "../n226_movStack/n226_500_movePicker.hpp"


struct Thread;


struct SplitPoint {
	// 局面か☆
	const Position*		m_position;

	const SearchStack*	m_searchStack;

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
