#pragma once

#include "../n080_common__/n080_105_time.hpp"
#include "../n119_score___/n119_200_pieceScore.hpp"
#include "../n220_position/n220_400_position.hpp"
#include "../n223_move____/n223_105_utilMove.hpp"
#include "../n223_move____/n223_300_moveScore.hpp"
#include "../n240_tt______/n240_300_tt.hpp"
#include "../n270_timeMng_/n270_100_timeManager.hpp"
#include "../n320_searcher/n320_125_searchStack.hpp"
#include "../n320_searcher/n320_128_signalsType.hpp"
#include "../n320_searcher/n320_131_inaniwaFlag.hpp"
#include "../n320_searcher/n320_134_bishopInDangerFlag.hpp"
#include "../n320_searcher/n320_137_rootMove.hpp"
#include "../n320_searcher/n320_140_stats.hpp"
#include "../n360_egOption/n360_230_engineOptionable.hpp"
#include "../n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../n450_thread__/n450_500_thread.hpp"

using namespace std;


struct SplitPoint;

using History = Stats<false>;
using Gains   = Stats<true>;

class TranspositionTable;

// 検索のための構造体？
class Searcher {
public:
	volatile SignalsType	m_signals;
	LimitsType				m_limits;
	std::vector<Move>		m_searchMoves;
	Time					m_searchTimer;
	StateStackPtr			m_setUpStates;
	std::vector<RootMove>	m_rootMoves;

#if defined LEARN
	Score					m_alpha;
	Score					m_beta;
#endif

	size_t					m_pvSize;
	size_t					m_pvIdx;
	TimeManager				m_timeManager;
	Ply						m_bestMoveChanges;
	History					m_history;
	Gains					m_gains;
	TranspositionTable		m_tt;

#if defined INANIWA_SHIFT
	InaniwaFlag				inaniwaFlag;
#endif
#if defined BISHOP_IN_DANGER
	BishopInDangerFlag		bishopInDangerFlag;
#endif
	Position				m_rootPosition;
	ThreadPool				m_threads;
	EngineOptionsMap		m_options;

	void					Init();
	void					IdLoop(Position& pos);
	std::string				PvInfoToUSI(Position& pos, const Ply depth, const Score alpha, const Score beta);

	template <NodeType NT, bool INCHECK>
	Score					Qsearch(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth);
#if defined INANIWA_SHIFT
	void					detectInaniwa(const Position& GetPos);
#endif
#if defined BISHOP_IN_DANGER
	void						detectBishopInDanger(const Position& GetPos);
#endif
	template <NodeType NT>
	Score					Search(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth, const bool cutNode);
	void					Think();
	void					CheckTime();

	void					SetOption(std::istringstream& ssCmd);
};

void InitSearchTable();

