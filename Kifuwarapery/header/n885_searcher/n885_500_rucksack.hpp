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
#include "../n350_pieceTyp/n350_040_ptEvent.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n560_timeMng_/n560_500_timeManager.hpp"
#include "../n640_searcher/n640_128_signalsType.hpp"
#include "../n640_searcher/n640_450_rootMove.hpp"
#include "../n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp"
#include "../n760_thread__/n760_400_herosPub.hpp"
#include "../n800_learn___/n800_100_stopwatch.hpp"
//#include "../n885_searcher/n885_600_iterativeDeepeningLoop.hpp"

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

	// 本譜の情報？
	std::string				PvInfoToUSI(Position& pos, const Ply depth, const ScoreIndex alpha, const ScoreIndex beta);

	// Ｑサーチ？
	template <NodeType NT, bool INCHECK>
	ScoreIndex					Qsearch(Position& pos, Flashlight* ss, ScoreIndex alpha, ScoreIndex beta, const Depth depth);

#if defined INANIWA_SHIFT
	void					detectInaniwa(const Position& GetPos);
#endif
#if defined BISHOP_IN_DANGER
	void						detectBishopInDanger(const Position& GetPos);
#endif

	// サーチ？
	template <NodeType NT>
	ScoreIndex					Search(Position& pos, Flashlight* ss, ScoreIndex alpha, ScoreIndex beta, const Depth depth, const bool cutNode);

	// 考える？
	void					Think();

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

	ScoreIndex scoreToTT(const ScoreIndex s, const Ply ply) {
		assert(s != ScoreNone);

		return (ScoreMateInMaxPly <= s ? s + static_cast<ScoreIndex>(ply)
			: s <= ScoreMatedInMaxPly ? s - static_cast<ScoreIndex>(ply)
			: s);
	}

	ScoreIndex scoreFromTT(const ScoreIndex s, const Ply ply) {
		return (s == ScoreNone ? ScoreNone
			: ScoreMateInMaxPly <= s ? s - static_cast<ScoreIndex>(ply)
			: s <= ScoreMatedInMaxPly ? s + static_cast<ScoreIndex>(ply)
			: s);
	}

	// fitst move によって、first move の相手側の second move を違法手にするか。
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

			const Color us = pos.GetTurn();
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

			if (g_setMaskBb.IsSet(&UtilAttack::GetAttacksFrom(m1ptTo, us, m1to, occ), m2to)) {
				return true;
			}

			const Color them = UtilColor::OppositeColor(us);
			// first で動いた後、sq へ当たりになっている遠隔駒
			const PieceTypeEvent ptEventL(occ, them, m2to);
			const PieceTypeEvent ptEventRB(occ, Color::ColorNum, m2to);
			const Bitboard xray =
				(PieceTypeArray::m_lance.GetAttacks2From(ptEventL) & pos.GetBbOf(N02_Lance, us))
				| (PieceTypeArray::m_rook.GetAttacks2From(ptEventRB) & pos.GetBbOf(N06_Rook, N14_Dragon, us))
				| (PieceTypeArray::m_bishop.GetAttacks2From(ptEventRB) & pos.GetBbOf(N05_Bishop, N13_Horse, us));

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


};

// メイン関数で１回だけ呼ばれる。
void InitSearchTable();


//────────────────────────────────────────────────────────────────────────────────
// 依存順の関係でここに☆（＾ｑ＾）
//────────────────────────────────────────────────────────────────────────────────

// 起きろ？
// 一箇所でしか呼ばないので、FORCE_INLINE
FORCE_INLINE void HerosPub::WakeUp(Rucksack* rucksack) {
	for (size_t i = 0; i < size(); ++i) {
		(*this)[i]->m_maxPly = 0;
	}
	this->m_isSleepWhileIdle_ = rucksack->m_engineOptions["Use_Sleeping_Threads"];
}

// 寝ろ？
// 一箇所でしか呼ばないので、FORCE_INLINE
FORCE_INLINE void HerosPub::Sleep() {
	this->m_isSleepWhileIdle_ = true;
}
