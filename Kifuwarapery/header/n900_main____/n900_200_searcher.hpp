#pragma once

#include <string>
#include <sstream>
#include "../n080_common__/n080_105_time.hpp"
#include "../n223_move____/n223_060_stats.hpp"
#include "../n228_movStack/n228_600_splitPoint.hpp"
#include "../n270_timeMng_/n270_100_timeManager.hpp"
#include "../n320_searcher/n320_128_signalsType.hpp"
#include "../n320_searcher/n320_137_rootMove.hpp"
#include "../n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../n450_thread__/n450_400_threadPool.hpp"

using namespace std;

using History = Stats<false>;
using Gains   = Stats<true>;




// 検索のための構造体？
class Searcher {
public:

	// シグナル？
	volatile SignalsType	m_signals;

	// リミッツ？
	LimitsType				m_limits;

	// ムーブ？
	std::vector<Move>		m_searchMoves;

	// タイマー？
	Time					m_searchTimer;

	// ステータス？
	StateStackPtr			m_setUpStates;

	// ルート？
	std::vector<RootMove>	m_rootMoves;

#if defined LEARN
	Score					m_alpha;
	Score					m_beta;
#endif

	// 本譜のサイズ？
	size_t					m_pvSize;

	// インデックス？
	size_t					m_pvIdx;

	// タイム・マネージャー？
	TimeManager				m_timeManager;

	// ベストムーブ？
	Ply						m_bestMoveChanges;

	// ヒストリー？
	History					m_history;

	// ゲインズ？
	Gains					m_gains;

	// トランジション・テーブル？
	TranspositionTable		m_tt;

#if defined INANIWA_SHIFT
	InaniwaFlag				inaniwaFlag;
#endif
#if defined BISHOP_IN_DANGER
	BishopInDangerFlag		bishopInDangerFlag;
#endif

	// 開始局面？
	Position				m_rootPosition;

	// スレッズ？
	ThreadPool				m_threads;

	// USIオプション？
	EngineOptionsMap		m_engineOptions;

	// 初期化？
	void					Init();

	// IDループ？
	void					IdLoop(Position& pos);

	// 本譜の情報？
	std::string				PvInfoToUSI(Position& pos, const Ply depth, const Score alpha, const Score beta);

	// Ｑサーチ？
	template <NodeType NT, bool INCHECK>
	Score					Qsearch(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth);

#if defined INANIWA_SHIFT
	void					detectInaniwa(const Position& GetPos);
#endif
#if defined BISHOP_IN_DANGER
	void						detectBishopInDanger(const Position& GetPos);
#endif

	// サーチ？
	template <NodeType NT>
	Score					Search(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth, const bool cutNode);

	// 考える？
	void					Think();

	// 時間チェック？
	void					CheckTime();

	// エンジン・オプション設定？
	void					SetOption(std::istringstream& ssCmd);
};

// メイン関数で１回だけ呼ばれる。
void InitSearchTable();

