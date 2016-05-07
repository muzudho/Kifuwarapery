#pragma once

#include <string>
#include <sstream>
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_130_lanceAttackBb.hpp"
#include "../n160_board___/n160_150_rookAttackBb.hpp"
#include "../n160_board___/n160_220_queenAttackBb.hpp"
#include "../n160_board___/n160_230_setMaskBb.hpp"
#include "../n223_move____/n223_060_stats.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_040_ptEvent.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_500_ptPrograms.hpp"
#include "../n560_timeMng_/n560_500_timeManager.hpp"
#include "../n640_searcher/n640_128_signalsType.hpp"
#include "../n640_searcher/n640_450_rootMove.hpp"
#include "../n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n760_thread__/n760_400_herosPub.hpp"
#include "../n800_stpWatch/n800_100_stopwatch.hpp"
//#include "../n885_searcher/n885_600_iterativeDeepeningLoop.hpp"
#include "n885_510_hitchhiker.hpp"	// FIXME:

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

	// ルート？ 前回の反復深化探索☆？（イテレーション）の結果が入っているみたいだぜ☆
	std::vector<RootMove>	m_rootMoves;

#if defined LEARN
	ScoreIndex					m_alpha;
	ScoreIndex					m_beta;
#endif

	// 本譜のサイズ？
	size_t					m_pvSize;

	// インデックス？
	size_t					m_pvIdx;

	// タイム・マネージャー？
	TimeManager				m_timeManager;

	//────────────────────────────────────────────────────────────────────────────────
	// ベストムーブ・チェンジスって何だぜ☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	inline Ply GetBestMovePlyChanges()
	{
		return this->m_bestMovePlyChanges_;
	}
	inline void ZeroclearBestMovePlyChanges()
	{
		this->m_bestMovePlyChanges_ = 0;
	}
	inline void IncreaseBestMovePlyChanges()
	{
		++this->m_bestMovePlyChanges_;
	}

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

	// 本譜の情報？
	std::string				PvInfoToUSI(Position& pos, const Ply depth, const ScoreIndex alpha, const ScoreIndex beta);

#if defined INANIWA_SHIFT
	void					detectInaniwa(const Position& GetPos);
#endif
#if defined BISHOP_IN_DANGER
	void						detectBishopInDanger(const Position& GetPos);
#endif


	// 時間チェック？
	void					CheckTime();

	// エンジン・オプション設定？
	void					SetOption(std::istringstream& ssCmd);



	//private:
	std::string scoreToUSI(const ScoreIndex score, const ScoreIndex alpha, const ScoreIndex beta) {
		std::stringstream ss;

		if (abs(score) < ScoreMateInMaxPly) {
			// cp は centi pawn の略
			ss << "cp " << score * 100 / PieceScore::m_pawn;
		}
		else {
			// mate の後には、何手で詰むかを表示する。
			ss << "mate " << (0 < score ? ScoreMate0Ply - score : -ScoreMate0Ply - score);
		}

		ss << (beta <= score ? " lowerbound" : score <= alpha ? " upperbound" : "");

		return ss.str();
	}

	//private:
	inline std::string scoreToUSI(const ScoreIndex score) {
		return scoreToUSI(score, -ScoreIndex::ScoreInfinite, ScoreIndex::ScoreInfinite);
	}

public://private:
	   // true にすると、シングルスレッドで動作する。デバッグ用。
	static const bool FakeSplit = false;

	inline ScoreIndex razorMargin(const Depth d) {
		return static_cast<ScoreIndex>(512 + 16 * static_cast<int>(d));
	}

	// checkTime() を呼び出す最大間隔(msec)
	const int TimerResolution = 5;


	inline bool checkIsDangerous() {
		// not implement
		// 使用しないで良いかも知れない。
		return false;
	}

	// 1 ply前の first move によって second move が合法手にするか。
	bool allows(const Position& pos, const Move first, const Move second) {
		const Square m1to = first.To();
		const Square m1from = first.From();
		const Square m2from = second.From();
		const Square m2to = second.To();
		if (m1to == m2from || m2to == m1from) {
			return true;
		}

		if (second.IsDrop() && first.IsDrop()) {
			return false;
		}

		if (!second.IsDrop() && !first.IsDrop()) {
			if (g_setMaskBb.IsSet(&g_betweenBb.GetBetweenBB(m2from, m2to), m1from)) {
				return true;
			}
		}

		const PieceType m1pt = first.GetPieceTypeFromOrDropped();
		const Color us = pos.GetTurn();
		const Bitboard occ = (second.IsDrop() ? pos.GetOccupiedBB() : pos.GetOccupiedBB() ^ g_setMaskBb.GetSetMaskBb(m2from));
		const Bitboard m1att = UtilAttack::GetAttacksFrom(m1pt, us, m1to, occ);
		if (g_setMaskBb.IsSet(&m1att, m2to)) {
			return true;
		}

		if (g_setMaskBb.IsSet(&m1att, pos.GetKingSquare(us))) {
			return true;
		}

		return false;
	}

	// トランスポジション・テーブル用にスコアを変更する？
	ScoreIndex ConvertScoreToTT(const ScoreIndex score, const Ply ply) {
		assert(score != ScoreNone);

		return (
			// mate表示をするとき☆
			ScoreMateInMaxPly <= score ?
			// スコアの土台に、手数（mate）を乗せるぜ☆！
			score + static_cast<ScoreIndex>(ply)
			:
			score <= ScoreMatedInMaxPly ?
			// 先後逆のときも、手数を乗せる（マイナスをもっと引く）のは同じ☆
			score - static_cast<ScoreIndex>(ply)
			// それ以外のときは、そのままスコア表示。
			: score
		);
	}

	// トランスポジション・テーブル用のスコアから、スコアを復元する？
	ScoreIndex ConvertScoreFromTT(const ScoreIndex s, const Ply ply) {
		return (s == ScoreNone ? ScoreNone
			: ScoreMateInMaxPly <= s ? s - static_cast<ScoreIndex>(ply)
			: s <= ScoreMatedInMaxPly ? s + static_cast<ScoreIndex>(ply)
			: s);
	}

	// fitst move によって、first move の相手側の second move を違法手にするか。
	template<Color US, Color THEM>
	bool refutes(const Position& pos, const Move first, const Move second) {
		assert(pos.IsOK());

		const Square m2to = second.To();
		const Square m1from = first.From(); // 駒打でも今回はこれで良い。

		if (m1from == m2to) {
			return true;
		}

		const PieceType m2ptFrom = second.GetPieceTypeFrom();
		if (
			second.IsCaptureOrPromotion()
			&& (
				//(pos.GetPieceScore(second.GetCap()) <= pos.GetPieceScore(m2ptFrom))
				(PieceScore::GetPieceScore(second.GetCap()) <= PieceScore::GetPieceScore(m2ptFrom))
				||
				m2ptFrom == N08_King
				)
			) {
			// first により、新たに m2to に当たりになる駒があるなら true
			assert(!second.IsDrop());

			//const Color us = pos.GetTurn();
			const Square m1to = first.To();
			const Square m2from = second.From();
			Bitboard occ = pos.GetOccupiedBB() ^ g_setMaskBb.GetSetMaskBb(m2from) ^ g_setMaskBb.GetSetMaskBb(m1to);
			PieceType m1ptTo;

			if (first.IsDrop()) {
				m1ptTo = first.GetPieceTypeDropped();
			}
			else {
				m1ptTo = first.GetPieceTypeTo();
				occ ^= g_setMaskBb.GetSetMaskBb(m1from);
			}

			if (g_setMaskBb.IsSet(&UtilAttack::GetAttacksFrom(m1ptTo, US, m1to, occ), m2to)) {
				return true;
			}

			// first で動いた後、sq へ当たりになっている遠隔駒
			const PieceTypeEvent ptEventL(occ, THEM, m2to);
			const PieceTypeEvent ptEventRB(occ, Color::Null, m2to);
			const Bitboard xray =
				(PiecetypePrograms::m_LANCE.GetAttacks2From(ptEventL) & pos.GetBbOf20<US>(N02_Lance))
				| (PiecetypePrograms::m_ROOK.GetAttacks2From(ptEventRB) & pos.GetBbOf30<US>(N06_Rook, N14_Dragon))
				| (PiecetypePrograms::m_BISHOP.GetAttacks2From(ptEventRB) & pos.GetBbOf30<US>(N05_Bishop, N13_Horse));

			// sq へ当たりになっている駒のうち、first で動くことによって新たに当たりになったものがあるなら true
			if (xray.Exists1Bit() && (xray ^ (xray & g_queenAttackBb.GetControllBb(pos.GetOccupiedBB(), m2to))).Exists1Bit()) {
				return true;
			}
		}

		if (!second.IsDrop()
			&& ConvPieceType::IS_SLIDER10(m2ptFrom)
			&& g_setMaskBb.IsSet(&g_betweenBb.GetBetweenBB(second.From(), m2to), first.To())
			&& ScoreZero <= pos.GetSeeSign(first))
		{
			return true;
		}

		return false;
	}


private:
	//────────────────────────────────────────────────────────────────────────────────
	// ベストムーブ・チェンジスって何だぜ☆？（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// 元の名前：ｂｅｓｔＭｏｖｅＣｈａｎｇｅｓ
	Ply						m_bestMovePlyChanges_;

};

// メイン関数で１回だけ呼ばれる。
void InitSearchTable();


//────────────────────────────────────────────────────────────────────────────────
// 依存順の関係でここに☆（＾ｑ＾）
//────────────────────────────────────────────────────────────────────────────────

// 起きろ？
// 一箇所でしか呼ばないので、FORCE_INLINE
FORCE_INLINE void HerosPub::WakeUp(Rucksack* rucksack) {
	// 全員初期化☆？
	for (size_t i = 0; i < size(); ++i) {
		(*this)[i]->m_maxPly = 0;
	}
	// フラグを立てる☆？
	this->m_isSleepWhileIdle_ = rucksack->m_engineOptions["Use_Sleeping_Threads"];
}

// 寝ろ？
// 一箇所でしか呼ばないので、FORCE_INLINE
FORCE_INLINE void HerosPub::Sleep() {
	this->m_isSleepWhileIdle_ = true;
}
