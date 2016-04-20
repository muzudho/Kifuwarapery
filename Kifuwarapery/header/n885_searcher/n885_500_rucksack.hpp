﻿#pragma once

#include <string>
#include <sstream>
#include "../n223_move____/n223_060_stats.hpp"
#include "../n560_timeMng_/n560_500_timeManager.hpp"
#include "../n640_searcher/n640_128_signalsType.hpp"
#include "../n640_searcher/n640_450_rootMove.hpp"
#include "../n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n760_thread__/n760_400_herosPub.hpp"
#include "../n800_learn___/n800_100_stopwatch.hpp"

using namespace std;

using History = Stats<false>;
using Gains   = Stats<true>;



// 元の名前：　Ｓｅａｒｃｈｅｒ
// 検索のための構造体？
//rucksack
class Rucksack {
public:

	// シグナル？
	volatile SignalsType	m_signals;

	// 思考用の時間などの上限☆？
	LimitsOfThinking		m_limits;

	// 元の名前：ｓｅａｒｃｈＭｏｖｅｓ
	// 送られてきた棋譜。（現局面までの指し手のスタック）
	std::vector<Move>		m_ourMoves;

	// 検索用タイマー？
	Stopwatch				m_stopwatch;

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
	HerosPub				m_ownerHerosPub;

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
	Score					Qsearch(Position& pos, Flashlight* ss, Score alpha, Score beta, const Depth depth);

#if defined INANIWA_SHIFT
	void					detectInaniwa(const Position& GetPos);
#endif
#if defined BISHOP_IN_DANGER
	void						detectBishopInDanger(const Position& GetPos);
#endif

	// サーチ？
	template <NodeType NT>
	Score					Search(Position& pos, Flashlight* ss, Score alpha, Score beta, const Depth depth, const bool cutNode);

	// 考える？
	void					Think();

	// 時間チェック？
	void					CheckTime();

	// エンジン・オプション設定？
	void					SetOption(std::istringstream& ssCmd);
};

// メイン関数で１回だけ呼ばれる。
void InitSearchTable();

