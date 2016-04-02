#pragma once

#include "../../header/n080_common__/n080_105_time.hpp"
#include "../../header/n200_score___/n200_200_pieceScore.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n240_position/n240_400_MoveScore.hpp"
#include "../../header/n270_timeMng_/n270_100_timeManager.hpp"
#include "../../header/n320_searcher/n320_125_searchStack.hpp"
#include "../../header/n320_searcher/n320_128_signalsType.hpp"
#include "../../header/n320_searcher/n320_131_inaniwaFlag.hpp"
#include "../../header/n320_searcher/n320_134_bishopInDangerFlag.hpp"
#include "../../header/n320_searcher/n320_137_rootMove.hpp"
#include "../../header/n320_searcher/n320_140_stats.hpp"
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"

using namespace std;

class Position;
struct SplitPoint;

using History = Stats<false>;
using Gains   = Stats<true>;

class TranspositionTable;

// 検索のための構造体？
class Searcher {
public:
	// static メンバ関数からだとthis呼べないので代わりに thisptr を使う。
	// static じゃないときは this を入れることにする。
	Searcher*				thisptr;
	volatile SignalsType	signals;
	LimitsType				limits;
	std::vector<Move>		searchMoves;
	Time					searchTimer;
	StateStackPtr			setUpStates;
	std::vector<RootMove>	rootMoves;

#if defined LEARN
	Score					alpha;
	Score					beta;
#endif

	size_t					pvSize;
	size_t					pvIdx;
	TimeManager				timeManager;
	Ply						bestMoveChanges;
	History					history;
	Gains					gains;
	TranspositionTable		tt;

#if defined INANIWA_SHIFT
	InaniwaFlag				inaniwaFlag;
#endif
#if defined BISHOP_IN_DANGER
	BishopInDangerFlag		bishopInDangerFlag;
#endif
	Position				rootPosition;
	ThreadPool				threads;
	EngineOptionsMap		options;

	void					init();
	void					idLoop(Position& pos);
	std::string				pvInfoToUSI(Position& pos, const Ply depth, const Score alpha, const Score beta);
	template <NodeType NT, bool INCHECK>
	Score					qsearch(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth);
#if defined INANIWA_SHIFT
	void					detectInaniwa(const Position& pos);
#endif
#if defined BISHOP_IN_DANGER
	void						detectBishopInDanger(const Position& pos);
#endif
	template <NodeType NT>
	Score					search(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth, const bool cutNode);
	void					think();
	void					checkTime();

	void					setOption(std::istringstream& ssCmd);
};

void initSearchTable();

