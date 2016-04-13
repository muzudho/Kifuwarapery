#include <iostream>
#include "../../header/n080_common__/n080_105_time.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_220_queenAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n220_position/n220_640_utilAttack.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_665_utilMoveStack.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_300_moveScore.hpp"
#include "../../header/n223_move____/n223_300_moveScore.hpp"
#include "../../header/n228_movStack/n228_500_movePicker.hpp"
#include "../../header/n520_evaluate/n520_700_evaluation09.hpp"
#include "../../header/n560_timeMng_/n560_500_timeManager.hpp"
#include "../../header/n600_book____/n600_500_book.hpp"
#include "../../header/n640_searcher/n640_500_reductions.hpp"
#include "../../header/n640_searcher/n640_510_futilityMargins.hpp"
#include "../../header/n640_searcher/n640_520_futilityMoveCounts.hpp"
#include "../../header/n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../../header/n680_egOption/n680_300_engineOptionSetup.hpp"
#include "../../header/n760_thread__/n760_400_threadPool.hpp"
#include "../../header/n900_main____/n900_200_searcher.hpp"

using namespace std;


extern const InFrontMaskBb g_inFrontMaskBb;


// 一箇所でしか呼ばないので、FORCE_INLINE
FORCE_INLINE void ThreadPool::WakeUp(Searcher* s) {
	for (size_t i = 0; i < size(); ++i) {
		(*this)[i]->m_maxPly = 0;
	}
	m_isSleepWhileIdle_ = s->m_engineOptions["Use_Sleeping_Threads"];
}
// 一箇所でしか呼ばないので、FORCE_INLINE
FORCE_INLINE void ThreadPool::Sleep() {
	m_isSleepWhileIdle_ = true;
}


void Searcher::Init() {
	EngineOptionSetup engineOptionSetup;
	engineOptionSetup.Initialize( &m_engineOptions, this);

	this->m_threads.Init(this);
	this->m_tt.SetSize(this->m_engineOptions["USI_Hash"]);
}

namespace {
	// true にすると、シングルスレッドで動作する。デバッグ用。
	const bool FakeSplit = false;

	inline Score razorMargin(const Depth d) {
		return static_cast<Score>(512 + 16 * static_cast<int>(d));
	}

	// checkTime() を呼び出す最大間隔(msec)
	const int TimerResolution = 5;

	struct Skill {
		Skill(const int l, const int mr)
			: level(l),
			  max_random_score_diff(static_cast<Score>(mr)),
			  best(Move::GetMoveNone()) {}
		~Skill() {}
		void swapIfEnabled(Searcher* s) {
			if (enabled()) {
				auto it = std::find(s->m_rootMoves.begin(),
									s->m_rootMoves.end(),
									(!best.IsNone() ? best : pickMove(s)));
				if (s->m_rootMoves.begin() != it)
					SYNCCOUT << "info string swap multipv 1, " << it - s->m_rootMoves.begin() + 1 << SYNCENDL;
				std::swap(s->m_rootMoves[0], *it);
			}
		}
		bool enabled() const { return level < 20 || max_random_score_diff != ScoreZero; }
		bool timeToPick(const int depth) const { return depth == 1 + level; }
		Move pickMove(Searcher* s) {
			// level については未対応。max_random_score_diff についてのみ対応する。
			if (max_random_score_diff != ScoreZero) {
				size_t i = 1;
				for (; i < s->m_pvSize; ++i) {
					if (max_random_score_diff < s->m_rootMoves[0].m_score_ - s->m_rootMoves[i].m_score_)
						break;
				}
				// 0 から i-1 までの間でランダムに選ぶ。
				std::uniform_int_distribution<size_t> dist(0, i-1);
				best = s->m_rootMoves[dist(g_randomTimeSeed)].m_pv_[0];
				return best;
			}
			best = s->m_rootMoves[0].m_pv_[0];
			return best;
		}

		int level;
		Score max_random_score_diff;
		Move best;
	};

	inline bool checkIsDangerous() {
		// not implement
		// 使用しないで良いかも知れない。
		return false;
	}

	// 1 ply前の first move によって second move が合法手にするか。
	bool allows(const Position& pos, const Move first, const Move second) {
		const Square m1to   = first.To();
		const Square m1from = first.From();
		const Square m2from = second.From();
		const Square m2to   = second.To();
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

	Score scoreToTT(const Score s, const Ply ply) {
		assert(s != ScoreNone);

		return (ScoreMateInMaxPly <= s ? s + static_cast<Score>(ply)
				: s <= ScoreMatedInMaxPly ? s - static_cast<Score>(ply)
				: s);
	}

	Score scoreFromTT(const Score s, const Ply ply) {
		return (s == ScoreNone ? ScoreNone
				: ScoreMateInMaxPly <= s ? s - static_cast<Score>(ply)
				: s <= ScoreMatedInMaxPly ? s + static_cast<Score>(ply)
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
		if (second.IsCaptureOrPromotion()
			&& ((pos.GetPieceScore(second.GetCap()) <= pos.GetPieceScore(m2ptFrom))
				|| m2ptFrom == N08_King))
		{
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
			const Bitboard xray =
				(g_ptLance.GetAttacks2From(occ, them, m2to) & pos.GetBbOf(N02_Lance, us))
				| (g_ptRook.GetAttacks2From(occ, Color::ColorNum, m2to) & pos.GetBbOf(N06_Rook, N14_Dragon, us))
				| (g_ptBishop.GetAttacks2From(occ, Color::ColorNum, m2to) & pos.GetBbOf(N05_Bishop, N13_Horse, us));

			// sq へ当たりになっている駒のうち、first で動くことによって新たに当たりになったものがあるなら true
			if (xray.Exists1Bit() && (xray ^ (xray & g_queenAttackBb.GetControllBb(&pos.GetOccupiedBB(),m2to))).Exists1Bit()) {
				return true;
			}
		}

		if (!second.IsDrop()
			&& UtilPieceType::IsSlider(m2ptFrom)
			&& g_setMaskBb.IsSet(&g_betweenBb.GetBetweenBB(second.From(), m2to), first.To())
			&& ScoreZero <= pos.GetSeeSign(first))
		{
			return true;
		}

		return false;
	}

	std::string scoreToUSI(const Score score, const Score alpha, const Score beta) {
		std::stringstream ss;

		if (abs(score) < ScoreMateInMaxPly) {
			// cp は centi pawn の略
			ss << "cp " << score * 100 / g_PawnScore;
		}
		else {
			// mate の後には、何手で詰むかを表示する。
			ss << "mate " << (0 < score ? ScoreMate0Ply - score : -ScoreMate0Ply - score);
		}

		ss << (beta <= score ? " lowerbound" : score <= alpha ? " upperbound" : "");

		return ss.str();
	}

	inline std::string scoreToUSI(const Score score) {
		return scoreToUSI(score, -ScoreInfinite, ScoreInfinite);
	}
}

std::string Searcher::PvInfoToUSI(Position& pos, const Ply depth, const Score alpha, const Score beta) {
	const int t = m_searchTimer.Elapsed();
	const size_t usiPVSize = m_pvSize;
	Ply selDepth = 0; // 選択的に読んでいる部分の探索深さ。
	std::stringstream ss;

	for (size_t i = 0; i < m_threads.size(); ++i) {
		if (selDepth < m_threads[i]->m_maxPly) {
			selDepth = m_threads[i]->m_maxPly;
		}
	}

	for (size_t i = usiPVSize-1; 0 <= static_cast<int>(i); --i) {
		const bool update = (i <= m_pvIdx);

		if (depth == 1 && !update) {
			continue;
		}

		const Ply d = (update ? depth : depth - 1);
		const Score s = (update ? m_rootMoves[i].m_score_ : m_rootMoves[i].m_prevScore_);

		ss << "info depth " << d
		   << " seldepth " << selDepth
		   << " score " << (i == m_pvIdx ? scoreToUSI(s, alpha, beta) : scoreToUSI(s))
		   << " nodes " << pos.GetNodesSearched()
		   << " nps " << (0 < t ? pos.GetNodesSearched() * 1000 / t : 0)
		   << " time " << t
		   << " multipv " << i + 1
		   << " pv ";

		for (int j = 0; !m_rootMoves[i].m_pv_[j].IsNone(); ++j) {
			ss << " " << m_rootMoves[i].m_pv_[j].ToUSI();
		}

		ss << std::endl;
	}
	return ss.str();
}

template <NodeType NT, bool INCHECK>
Score Searcher::Qsearch(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth) {
	const bool PVNode = (NT == PV);

	assert(NT == PV || NT == NonPV);
	assert(INCHECK == pos.InCheck());
	assert(-ScoreInfinite <= alpha && alpha < beta && beta <= ScoreInfinite);
	assert(PVNode || (alpha == beta - 1));
	assert(depth <= Depth0);

	StateInfo st;
	const TTEntry* tte;
	Key posKey;
	Move ttMove;
	Move move;
	Move bestMove;
	Score bestScore;
	Score score;
	Score ttScore;
	Score futilityScore;
	Score futilityBase;
	Score oldAlpha;
	bool givesCheck;
	bool evasionPrunable;
	Depth ttDepth;

	if (PVNode) {
		oldAlpha = alpha;
	}

	ss->m_currentMove = bestMove = Move::GetMoveNone();
	ss->m_ply = (ss-1)->m_ply + 1;

	if (g_maxPly < ss->m_ply) {
		return ScoreDraw;
	}

	ttDepth = ((INCHECK || DepthQChecks <= depth) ? DepthQChecks : DepthQNoChecks);

	posKey = pos.GetKey();
	tte = m_tt.Probe(posKey);
	ttMove = (tte != nullptr ? UtilMoveStack::Move16toMove(tte->GetMove(), pos) : Move::GetMoveNone());
	ttScore = (tte != nullptr ? scoreFromTT(tte->GetScore(), ss->m_ply) : ScoreNone);

	if (tte != nullptr
		&& ttDepth <= tte->GetDepth()
		&& ttScore != ScoreNone // アクセス競合が起きたときのみ、ここに引っかかる。
		&& (PVNode ? tte->GetType() == BoundExact
			: (beta <= ttScore ? (tte->GetType() & BoundLower)
			   : (tte->GetType() & BoundUpper))))
	{
		ss->m_currentMove = ttMove;
		return ttScore;
	}

	pos.SetNodesSearched(pos.GetNodesSearched() + 1);

	if (INCHECK) {
		ss->m_staticEval = ScoreNone;
		bestScore = futilityBase = -ScoreInfinite;
	}
	else {
		if (!(move = pos.GetMateMoveIn1Ply()).IsNone()) {
			return UtilScore::MateIn(ss->m_ply);
		}

		if (tte != nullptr) {
			if ((ss->m_staticEval = bestScore = tte->GetEvalScore()) == ScoreNone) {
				Evaluation09 evaluation;
				ss->m_staticEval = bestScore = evaluation.evaluate(pos, ss);
			}
		}
		else {
			Evaluation09 evaluation;
			ss->m_staticEval = bestScore = evaluation.evaluate(pos, ss);
		}

		if (beta <= bestScore) {
			if (tte == nullptr) {
				m_tt.Store(pos.GetKey(), scoreToTT(bestScore, ss->m_ply), BoundLower,
						 DepthNone, Move::GetMoveNone(), ss->m_staticEval);
			}

			return bestScore;
		}

		if (PVNode && alpha < bestScore) {
			alpha = bestScore;
		}

		futilityBase = bestScore + 128; // todo: 128 より大きくて良いと思う。
	}

	Evaluation09 evaluation;
	evaluation.evaluate(pos, ss);

	MovePicker mp(pos, ttMove, depth, m_history, (ss-1)->m_currentMove.To());
	const CheckInfo ci(pos);

	while (!(move = mp.GetNextMove<false>()).IsNone())
	{
		assert(pos.IsOK());

		givesCheck = pos.IsMoveGivesCheck(move, ci);

		// futility pruning
		if (!PVNode
			&& !INCHECK // 駒打ちは王手回避のみなので、ここで弾かれる。
			&& !givesCheck
			&& move != ttMove)
		{
			futilityScore =
				futilityBase + Position::GetCapturePieceScore(pos.GetPiece(move.To()));
			if (move.IsPromotion()) {
				futilityScore += Position::GetPromotePieceScore(move.GetPieceTypeFrom());
			}

			if (futilityScore < beta) {
				bestScore = std::max(bestScore, futilityScore);
				continue;
			}

			// todo: MovePicker のオーダリングで SEE してるので、ここで SEE するの勿体無い。
			if (futilityBase < beta
				&& depth < Depth0
				&& pos.GetSee(move, beta - futilityBase) <= ScoreZero)
			{
				bestScore = std::max(bestScore, futilityBase);
				continue;
			}
		}

		evasionPrunable = (INCHECK
						   && ScoreMatedInMaxPly < bestScore
						   && !move.IsCaptureOrPawnPromotion());

		if (!PVNode
			&& (!INCHECK || evasionPrunable)
			&& move != ttMove
			&& (!move.IsPromotion() || move.GetPieceTypeFrom() != N01_Pawn)
			&& pos.GetSeeSign(move) < 0)
		{
			continue;
		}

		if (!pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
			continue;
		}

		ss->m_currentMove = move;

		pos.DoMove(move, st, ci, givesCheck);
		(ss+1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
		score = (givesCheck ? -Qsearch<NT, true>(pos, ss+1, -beta, -alpha, depth - OnePly)
				 : -Qsearch<NT, false>(pos, ss+1, -beta, -alpha, depth - OnePly));
		pos.UndoMove(move);

		assert(-ScoreInfinite < score && score < ScoreInfinite);

		if (bestScore < score) {
			bestScore = score;

			if (alpha < score) {
				if (PVNode && score < beta) {
					alpha = score;
					bestMove = move;
				}
				else {
					// fail high
					m_tt.Store(posKey, scoreToTT(score, ss->m_ply), BoundLower,
							 ttDepth, move, ss->m_staticEval);
					return score;
				}
			}
		}
	}

	if (INCHECK && bestScore == -ScoreInfinite) {
		return UtilScore::MatedIn(ss->m_ply);
	}

	m_tt.Store(posKey, scoreToTT(bestScore, ss->m_ply), 
			 ((PVNode && oldAlpha < bestScore) ? BoundExact : BoundUpper),
			 ttDepth, bestMove, ss->m_staticEval);

	assert(-ScoreInfinite < bestScore && bestScore < ScoreInfinite);

	return bestScore;
}

// iterative deepening loop
void Searcher::IdLoop(Position& pos) {
	SearchStack ss[g_maxPlyPlus2];
	Ply depth;
	Ply prevBestMoveChanges;
	Score bestScore = -ScoreInfinite;
	Score delta = -ScoreInfinite;
	Score alpha = -ScoreInfinite;
	Score beta = ScoreInfinite;
	bool bestMoveNeverChanged = true;
	int lastInfoTime = -1; // 将棋所のコンソールが詰まる問題への対処用

	memset(ss, 0, 4 * sizeof(SearchStack));
	m_bestMoveChanges = 0;
#if defined LEARN
	// 高速化の為に浅い探索は反復深化しないようにする。学習時は浅い探索をひたすら繰り返す為。
	GetDepth = std::max<Ply>(0, m_limits.GetDepth - 1);
#else
	depth = 0;
#endif

	ss[0].m_currentMove = Move::GetMoveNull(); // skip update gains
	m_tt.NewSearch();
	m_history.Clear();
	m_gains.Clear();

	m_pvSize = m_engineOptions["MultiPV"];
	Skill skill(m_engineOptions["Skill_Level"], m_engineOptions["Max_Random_Score_Diff"]);

	if (m_engineOptions["Max_Random_Score_Diff_Ply"] < pos.GetGamePly()) {
		skill.max_random_score_diff = ScoreZero;
		m_pvSize = 1;
		assert(!skill.enabled()); // level による設定が出来るようになるまでは、これで良い。
	}

	if (skill.enabled() && m_pvSize < 3) {
		m_pvSize = 3;
	}
	m_pvSize = std::min(m_pvSize, m_rootMoves.size());

	// 指し手が無ければ負け
	if (m_rootMoves.empty()) {
		m_rootMoves.push_back(RootMove(Move::GetMoveNone()));
		SYNCCOUT << "info depth 0 score "
				 << scoreToUSI(-ScoreMate0Ply)
				 << SYNCENDL;

		return;
	}

#if defined BISHOP_IN_DANGER
	if ((bishopInDangerFlag == BlackBishopInDangerIn28
		 && std::find_if(std::begin(m_rootMoves), std::IsEnd(m_rootMoves),
						 [](const RootMove& rm) { return rm.m_pv_[0].ToCSA() == "0082KA"; }) != std::IsEnd(m_rootMoves))
		|| (bishopInDangerFlag == WhiteBishopInDangerIn28
			&& std::find_if(std::begin(m_rootMoves), std::IsEnd(m_rootMoves),
							[](const RootMove& rm) { return rm.m_pv_[0].ToCSA() == "0028KA"; }) != std::IsEnd(m_rootMoves))
		|| (bishopInDangerFlag == BlackBishopInDangerIn78
			&& std::find_if(std::begin(m_rootMoves), std::IsEnd(m_rootMoves),
						 [](const RootMove& rm) { return rm.m_pv_[0].ToCSA() == "0032KA"; }) != std::IsEnd(m_rootMoves))
		|| (bishopInDangerFlag == WhiteBishopInDangerIn78
			&& std::find_if(std::begin(m_rootMoves), std::IsEnd(m_rootMoves),
							[](const RootMove& rm) { return rm.m_pv_[0].ToCSA() == "0078KA"; }) != std::IsEnd(m_rootMoves)))
	{
		if (m_rootMoves.m_size() != 1)
			m_pvSize = std::max<size_t>(m_pvSize, 2);
	}
#endif

	// 反復深化で探索を行う。
	while (++depth <= g_maxPly && !m_signals.m_stop && (!m_limits.m_depth || depth <= m_limits.m_depth)) {
		// 前回の iteration の結果を全てコピー
		for (size_t i = 0; i < m_rootMoves.size(); ++i) {
			m_rootMoves[i].m_prevScore_ = m_rootMoves[i].m_score_;
		}

		prevBestMoveChanges = m_bestMoveChanges;
		m_bestMoveChanges = 0;

		// Multi PV loop
		for (m_pvIdx = 0; m_pvIdx < m_pvSize && !m_signals.m_stop; ++m_pvIdx) {
#if defined LEARN
			m_alpha = this->m_alpha;
			m_beta  = this->m_beta;
#else
			// aspiration search
			// alpha, beta をある程度絞ることで、探索効率を上げる。
			if (5 <= depth && abs(m_rootMoves[m_pvIdx].m_prevScore_) < g_ScoreKnownWin) {
				delta = static_cast<Score>(16);
				alpha = m_rootMoves[m_pvIdx].m_prevScore_ - delta;
				beta  = m_rootMoves[m_pvIdx].m_prevScore_ + delta;
			}
			else {
				alpha = -ScoreInfinite;
				beta  =  ScoreInfinite;
			}
#endif

			// aspiration search の window 幅を、初めは小さい値にして探索し、
			// fail high/low になったなら、今度は window 幅を広げて、再探索を行う。
			while (true) {
				// 探索を行う。
				ss->m_staticEvalRaw.m_p[0][0] = (ss+1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
				bestScore = Search<Root>(pos, ss + 1, alpha, beta, static_cast<Depth>(depth * OnePly), false);
				// 先頭が最善手になるようにソート
				UtilMoveStack::InsertionSort(m_rootMoves.begin() + m_pvIdx, m_rootMoves.end());

				for (size_t i = 0; i <= m_pvIdx; ++i) {
					ss->m_staticEvalRaw.m_p[0][0] = (ss+1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
					m_rootMoves[i].InsertPvInTT(pos);
				}

#if 0
				// 詰みを発見したら即指す。
				if (ScoreMateInMaxPly <= abs(m_bestScore) && abs(m_bestScore) < ScoreInfinite) {
					SYNCCOUT << PvInfoToUSI(GetPos, m_ply, m_alpha, m_beta) << SYNCENDL;
					m_signals.m_stop = true;
				}
#endif

#if defined LEARN
				break;
#endif

				if (m_signals.m_stop) {
					break;
				}

				if (alpha < bestScore && bestScore < beta) {
					break;
				}

				if (3000 < m_searchTimer.Elapsed()
					// 将棋所のコンソールが詰まるのを防ぐ。
					&& (depth < 10 || lastInfoTime + 200 < m_searchTimer.Elapsed()))
				{
					lastInfoTime = m_searchTimer.Elapsed();
					SYNCCOUT << PvInfoToUSI(pos, depth, alpha, beta) << SYNCENDL;
				}

				// fail high/low のとき、aspiration window を広げる。
				if (g_ScoreKnownWin <= abs(bestScore)) {
					// 勝ち(負け)だと判定したら、最大の幅で探索を試してみる。
					alpha = -ScoreInfinite;
					beta = ScoreInfinite;
				}
				else if (beta <= bestScore) {
					beta += delta;
					delta += delta / 2;
				}
				else {
					m_signals.m_failedLowAtRoot = true;
					m_signals.m_stopOnPonderHit = false;

					alpha -= delta;
					delta += delta / 2;
				}

				assert(-ScoreInfinite <= alpha && beta <= ScoreInfinite);
			}

			UtilMoveStack::InsertionSort(m_rootMoves.begin(), m_rootMoves.begin() + m_pvIdx + 1);
			if ((m_pvIdx + 1 == m_pvSize || 3000 < m_searchTimer.Elapsed())
				// 将棋所のコンソールが詰まるのを防ぐ。
				&& (depth < 10 || lastInfoTime + 200 < m_searchTimer.Elapsed()))
			{
				lastInfoTime = m_searchTimer.Elapsed();
				SYNCCOUT << PvInfoToUSI(pos, depth, alpha, beta) << SYNCENDL;
			}
		}

		//if (skill.enabled() && skill.timeToPick(depth)) {
		//	skill.pickMove();
		//}

		if (m_limits.IsUseTimeManagement() && !m_signals.m_stopOnPonderHit) {
			bool stop = false;

			if (4 < depth && depth < 50 && m_pvSize == 1) {
				m_timeManager.PvInstability(m_bestMoveChanges, prevBestMoveChanges);
			}

			// 次のイテレーションを回す時間が無いなら、ストップ
			if ((m_timeManager.AvailableTime() * 62) / 100 < m_searchTimer.Elapsed()) {
				stop = true;
			}

			if (2 < depth && m_bestMoveChanges)
				bestMoveNeverChanged = false;

			// 最善手が、ある程度の深さまで同じであれば、
			// その手が突出して良い手なのだろう。
			if (12 <= depth
				&& !stop
				&& bestMoveNeverChanged
				&& m_pvSize == 1
				// ここは確実にバグらせないようにする。
				&& -ScoreInfinite + 2 * g_CapturePawnScore <= bestScore
				&& (m_rootMoves.size() == 1
					|| m_timeManager.AvailableTime() * 40 / 100 < m_searchTimer.Elapsed()))
			{
				const Score rBeta = bestScore - 2 * g_CapturePawnScore;
				(ss+1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
				(ss+1)->m_excludedMove = m_rootMoves[0].m_pv_[0];
				(ss+1)->m_skipNullMove = true;
				const Score s = Search<NonPV>(pos, ss+1, rBeta-1, rBeta, (depth - 3) * OnePly, true);
				(ss+1)->m_skipNullMove = false;
				(ss+1)->m_excludedMove = Move::GetMoveNone();

				if (s < rBeta) {
					stop = true;
				}
			}

			if (stop) {
				if (m_limits.m_ponder) {
					m_signals.m_stopOnPonderHit = true;
				}
				else {
					m_signals.m_stop = true;
				}
			}
		}
	}
	skill.swapIfEnabled(this);
	SYNCCOUT << PvInfoToUSI(pos, depth-1, alpha, beta) << SYNCENDL;
}

#if defined INANIWA_SHIFT
// 稲庭判定
void Searcher::detectInaniwa(const Position& GetPos) {
	if (inaniwaFlag == NotInaniwa && 20 <= GetPos.GetGamePly()) {
		const Rank TRank7 = (GetPos.GetTurn() == Black ? Rank7 : Rank3); // not constant
		const Bitboard mask = g_rankMaskBb.GetRankMask(TRank7) & ~g_fileMaskBb.GetFileMask<FileA>() & ~g_fileMaskBb.GetFileMask<FileI>();
		if ((GetPos.GetBbOf(N01_Pawn, UtilColor::OppositeColor(GetPos.GetTurn())) & mask) == mask) {
			inaniwaFlag = (GetPos.GetTurn() == Black ? InaniwaIsWhite : InaniwaIsBlack);
			m_tt.Clear();
		}
	}
}
#endif
#if defined BISHOP_IN_DANGER
void Searcher::detectBishopInDanger(const Position& GetPos) {
	if (bishopInDangerFlag == NotBishopInDanger && GetPos.GetGamePly() <= 50) {
		const Color them = UtilColor::OppositeColor(GetPos.GetTurn());
		if (GetPos.m_hand(GetPos.GetTurn()).Exists<HBishop>()
			&& GetPos.GetBbOf(N04_Silver, them).IsSet(InverseIfWhite(them, H3))
			&& (GetPos.GetBbOf(N08_King  , them).IsSet(InverseIfWhite(them, F2))
				|| GetPos.GetBbOf(N08_King  , them).IsSet(InverseIfWhite(them, F3))
				|| GetPos.GetBbOf(N08_King  , them).IsSet(InverseIfWhite(them, E1)))
			&& GetPos.GetBbOf(N01_Pawn  , them).IsSet(InverseIfWhite(them, G3))
			&& GetPos.GetPiece(InverseIfWhite(them, H2)) == Empty
			&& GetPos.GetPiece(InverseIfWhite(them, G2)) == Empty
			&& GetPos.GetPiece(InverseIfWhite(them, G1)) == Empty)
		{
			bishopInDangerFlag = (GetPos.GetTurn() == Black ? BlackBishopInDangerIn28 : WhiteBishopInDangerIn28);
			//tt.clear();
		}
		else if (GetPos.m_hand(GetPos.GetTurn()).Exists<HBishop>()
				 && GetPos.m_hand(them).Exists<HBishop>()
				 && GetPos.GetPiece(InverseIfWhite(them, C2)) == Empty
				 && GetPos.GetPiece(InverseIfWhite(them, C1)) == Empty
				 && GetPos.GetPiece(InverseIfWhite(them, D2)) == Empty
				 && GetPos.GetPiece(InverseIfWhite(them, D1)) == Empty
				 && GetPos.GetPiece(InverseIfWhite(them, A2)) == Empty
				 && (UtilPiece::ToPieceType(GetPos.GetPiece(InverseIfWhite(them, C3))) == N04_Silver
					 || UtilPiece::ToPieceType(GetPos.GetPiece(InverseIfWhite(them, B2))) == N04_Silver)
				 && (UtilPiece::ToPieceType(GetPos.GetPiece(InverseIfWhite(them, C3))) == N03_Knight
					 || UtilPiece::ToPieceType(GetPos.GetPiece(InverseIfWhite(them, B1))) == N03_Knight)
				 && ((UtilPiece::ToPieceType(GetPos.GetPiece(InverseIfWhite(them, E2))) == N07_Gold
					  && UtilPiece::ToPieceType(GetPos.GetPiece(InverseIfWhite(them, E1))) == N08_King)
					 || UtilPiece::ToPieceType(GetPos.GetPiece(InverseIfWhite(them, E1))) == N07_Gold))
		{
			bishopInDangerFlag = (GetPos.GetTurn() == Black ? BlackBishopInDangerIn78 : WhiteBishopInDangerIn78);
			//tt.clear();
		}
	}
}
#endif
template <bool DO> void Position::DoNullMove(StateInfo& backUpSt) {
	assert(!InCheck());

	StateInfo* src = (DO ? m_st_ : &backUpSt);
	StateInfo* dst = (DO ? &backUpSt : m_st_);

	dst->m_boardKey      = src->m_boardKey;
	dst->m_handKey       = src->m_handKey;
	dst->m_pliesFromNull = src->m_pliesFromNull;
	dst->m_hand = GetHand(GetTurn());
	m_turn_ = UtilColor::OppositeColor(GetTurn());

	if (DO) {
		m_st_->m_boardKey ^= GetZobTurn();
		prefetch(GetCsearcher()->m_tt.FirstEntry(m_st_->GetKey()));
		m_st_->m_pliesFromNull = 0;
		m_st_->m_continuousCheck[GetTurn()] = 0;
	}
	m_st_->m_hand = GetHand(GetTurn());

	assert(IsOK());
}

template <NodeType NT>
Score Searcher::Search(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth, const bool cutNode) {
	const bool PVNode = (NT == PV || NT == Root || NT == SplitPointPV || NT == SplitPointRoot);
	const bool SPNode = (NT == SplitPointPV || NT == SplitPointNonPV || NT == SplitPointRoot);
	const bool RootNode = (NT == Root || NT == SplitPointRoot);

	assert(-ScoreInfinite <= alpha && alpha < beta && beta <= ScoreInfinite);
	assert(PVNode || (alpha == beta - 1));
	assert(Depth0 < depth);

	// 途中で goto を使用している為、先に全部の変数を定義しておいた方が安全。
	Move movesSearched[64];
	StateInfo st;
	const TTEntry* tte;
	SplitPoint* splitPoint;
	Key posKey;
	Move ttMove;
	Move move;
	Move excludedMove;
	Move bestMove;
	Move threatMove;
	Depth newDepth;
	Depth extension;
	Score bestScore;
	Score score;
	Score ttScore;
	Score eval;
	bool inCheck;
	bool givesCheck;
	bool isPVMove;
	bool singularExtensionNode;
	bool captureOrPawnPromotion;
	bool dangerous;
	bool doFullDepthSearch;
	int moveCount;
	int playedMoveCount;

	// step1
	// initialize node
	Thread* thisThread = pos.GetThisThread();
	moveCount = playedMoveCount = 0;
	inCheck = pos.InCheck();

	if (SPNode) {
		splitPoint = ss->m_splitPoint;
		bestMove = splitPoint->m_bestMove;
		threatMove = splitPoint->m_threatMove;
		bestScore = splitPoint->m_bestScore;
		tte = nullptr;
		ttMove = excludedMove = Move::GetMoveNone();
		ttScore = ScoreNone;

		Evaluation09 evaluation;
		evaluation.evaluate(pos, ss);

		assert(-ScoreInfinite < splitPoint->m_bestScore && 0 < splitPoint->m_moveCount);

		goto split_point_start;
	}

	bestScore = -ScoreInfinite;
	ss->m_currentMove = threatMove = bestMove = (ss + 1)->m_excludedMove = Move::GetMoveNone();
	ss->m_ply = (ss-1)->m_ply + 1;
	(ss+1)->m_skipNullMove = false;
	(ss+1)->m_reduction = Depth0;
	(ss+2)->m_killers[0] = (ss+2)->m_killers[1] = Move::GetMoveNone();

	if (PVNode && thisThread->m_maxPly < ss->m_ply) {
		thisThread->m_maxPly = ss->m_ply;
	}

	if (!RootNode) {
		// step2
		// stop と最大探索深さのチェック
		switch (pos.IsDraw(16)) {
		case NotRepetition      : if (!m_signals.m_stop && ss->m_ply <= g_maxPly) { break; }
		case RepetitionDraw     : return ScoreDraw;
		case RepetitionWin      : return UtilScore::MateIn(ss->m_ply);
		case RepetitionLose     : return UtilScore::MatedIn(ss->m_ply);
		case RepetitionSuperior : if (ss->m_ply != 2) { return ScoreMateInMaxPly; } break;
		case RepetitionInferior : if (ss->m_ply != 2) { return ScoreMatedInMaxPly; } break;
		default                 : UNREACHABLE;
		}

		// step3
		// mate distance pruning
		if (!RootNode) {
			alpha = std::max(UtilScore::MatedIn(ss->m_ply), alpha);
			beta = std::min(UtilScore::MateIn(ss->m_ply+1), beta);
			if (beta <= alpha) {
				return alpha;
			}
		}
	}

	pos.SetNodesSearched(pos.GetNodesSearched() + 1);

	// step4
	// trans position table lookup
	excludedMove = ss->m_excludedMove;
	posKey = (excludedMove.IsNone() ? pos.GetKey() : pos.GetExclusionKey());
	tte = m_tt.Probe(posKey);
	ttMove = 
		RootNode ? m_rootMoves[m_pvIdx].m_pv_[0] :
		tte != nullptr ?
		UtilMoveStack::Move16toMove(tte->GetMove(), pos) :
		Move::GetMoveNone();
	ttScore = (tte != nullptr ? scoreFromTT(tte->GetScore(), ss->m_ply) : ScoreNone);

	if (!RootNode
		&& tte != nullptr
		&& depth <= tte->GetDepth()
		&& ttScore != ScoreNone // アクセス競合が起きたときのみ、ここに引っかかる。
		&& (PVNode ? tte->GetType() == BoundExact
			: (beta <= ttScore ? (tte->GetType() & BoundLower)
			   : (tte->GetType() & BoundUpper))))
	{
		m_tt.Refresh(tte);
		ss->m_currentMove = ttMove; // Move::moveNone() もありえる。

		if (beta <= ttScore
			&& !ttMove.IsNone()
			&& !ttMove.IsCaptureOrPawnPromotion()
			&& ttMove != ss->m_killers[0])
		{
			ss->m_killers[1] = ss->m_killers[0];
			ss->m_killers[0] = ttMove;
		}
		return ttScore;
	}

#if 1
	if (!RootNode
		&& !inCheck)
	{
		if (!(move = pos.GetMateMoveIn1Ply()).IsNone()) {
			ss->m_staticEval = bestScore = UtilScore::MateIn(ss->m_ply);
			m_tt.Store(posKey, scoreToTT(bestScore, ss->m_ply), BoundExact, depth,
					 move, ss->m_staticEval);
			bestMove = move;
			return bestScore;
		}
	}
#endif

	// step5
	// evaluate the position statically
	Evaluation09 evaluation;
	eval = ss->m_staticEval = evaluation.evaluate(pos, ss); // Bonanza の差分評価の為、evaluate() を常に呼ぶ。
	if (inCheck) {
		eval = ss->m_staticEval = ScoreNone;
		goto iid_start;
	}
	else if (tte != nullptr) {
		if (ttScore != ScoreNone
			&& (tte->GetType() & (eval < ttScore ? BoundLower : BoundUpper)))
		{
			eval = ttScore;
		}
	}
	else {
		m_tt.Store(posKey, ScoreNone, BoundNone, DepthNone,
				 Move::GetMoveNone(), ss->m_staticEval);
	}

	// 一手前の指し手について、history を更新する。
	// todo: ここの条件はもう少し考えた方が良い。
	if ((move = (ss-1)->m_currentMove) != Move::GetMoveNull()
		&& (ss-1)->m_staticEval != ScoreNone
		&& ss->m_staticEval != ScoreNone
		&& !move.IsCaptureOrPawnPromotion() // 前回(一手前)の指し手が駒取りでなかった。
		)
	{
		const Square to = move.To();
		m_gains.Update(move.IsDrop(), pos.GetPiece(to), to, -(ss-1)->m_staticEval - ss->m_staticEval);
	}

	// step6
	// razoring
	if (!PVNode
		&& depth < 4 * OnePly
		&& eval + razorMargin(depth) < beta
		&& ttMove.IsNone()
		&& abs(beta) < ScoreMateInMaxPly)
	{
		const Score rbeta = beta - razorMargin(depth);
		const Score s = Qsearch<NonPV, false>(pos, ss, rbeta-1, rbeta, Depth0);
		if (s < rbeta) {
			return s;
		}
	}

	// step7
	// static null move pruning
	if (!PVNode
		&& !ss->m_skipNullMove
		&& depth < 4 * OnePly
		&& beta <= eval - g_futilityMargins.m_FutilityMargins[depth][0]
		&& abs(beta) < ScoreMateInMaxPly)
	{
		return eval - g_futilityMargins.m_FutilityMargins[depth][0];
	}

	// step8
	// null move
	if (!PVNode
		&& !ss->m_skipNullMove
		&& 2 * OnePly <= depth
		&& beta <= eval
		&& abs(beta) < ScoreMateInMaxPly)
	{
		ss->m_currentMove = Move::GetMoveNull();
		Depth reduction = static_cast<Depth>(3) * OnePly + depth / 4;

		if (beta < eval - g_PawnScore) {
			reduction += OnePly;
		}

		pos.DoNullMove<true>(st);
		(ss+1)->m_staticEvalRaw = (ss)->m_staticEvalRaw; // 評価値の差分評価の為。
		(ss+1)->m_skipNullMove = true;
		Score nullScore = (depth - reduction < OnePly ?
						   -Qsearch<NonPV, false>(pos, ss + 1, -beta, -alpha, Depth0)
						   : -Search<NonPV>(pos, ss + 1, -beta, -alpha, depth - reduction, !cutNode));
		(ss+1)->m_skipNullMove = false;
		pos.DoNullMove<false>(st);

		if (beta <= nullScore) {
			if (ScoreMateInMaxPly <= nullScore) {
				nullScore = beta;
			}

			if (depth < 6 * OnePly) {
				return nullScore;
			}

			ss->m_skipNullMove = true;
			assert(Depth0 < depth - reduction);
			const Score s = Search<NonPV>(pos, ss, alpha, beta, depth - reduction, false);
			ss->m_skipNullMove = false;

			if (beta <= s) {
				return nullScore;
			}
		}
		else {
			// fail low
			threatMove = (ss+1)->m_currentMove;
			if (depth < 5 * OnePly
				&& (ss-1)->m_reduction != Depth0
				&& !threatMove.IsNone()
				&& allows(pos, (ss-1)->m_currentMove, threatMove))
			{
				return beta - 1;
			}
		}
	}

	// step9
	// probcut
	if (!PVNode
		&& 5 * OnePly <= depth
		&& !ss->m_skipNullMove
		// 確実にバグらせないようにする。
		&& abs(beta) < ScoreInfinite - 200)
	{
		const Score rbeta = beta + 200;
		const Depth rdepth = depth - OnePly - 3 * OnePly;

		assert(OnePly <= rdepth);
		assert(!(ss-1)->m_currentMove.IsNone());
		assert((ss-1)->m_currentMove != Move::GetMoveNull());

		assert(move == (ss-1)->m_currentMove);
		// move.cap() は前回(一手前)の指し手で取った駒の種類
		MovePicker mp(pos, ttMove, m_history, move.GetCap());
		const CheckInfo ci(pos);
		while (!(move = mp.GetNextMove<false>()).IsNone()) {
			if (pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
				ss->m_currentMove = move;
				pos.DoMove(move, st, ci, pos.IsMoveGivesCheck(move, ci));
				(ss+1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
				score = -Search<NonPV>(pos, ss+1, -rbeta, -rbeta+1, rdepth, !cutNode);
				pos.UndoMove(move);
				if (rbeta <= score) {
					return score;
				}
			}
		}
	}

iid_start:
	// step10
	// internal iterative deepening
	if ((PVNode ? 5 * OnePly : 8 * OnePly) <= depth
		&& ttMove.IsNone()
		&& (PVNode || (!inCheck && beta <= ss->m_staticEval + static_cast<Score>(256))))
	{
		//const Depth d = depth - 2 * OnePly - (PVNode ? Depth0 : depth / 4);
		const Depth d = (PVNode ? depth - 2 * OnePly : depth / 2);

		ss->m_skipNullMove = true;
		Search<PVNode ? PV : NonPV>(pos, ss, alpha, beta, d, true);
		ss->m_skipNullMove = false;

		tte = m_tt.Probe(posKey);
		ttMove = (tte != nullptr ?
			UtilMoveStack::Move16toMove(tte->GetMove(), pos) :
				  Move::GetMoveNone());
	}

split_point_start:
	MovePicker mp(pos, ttMove, depth, m_history, ss, PVNode ? -ScoreInfinite : beta);
	const CheckInfo ci(pos);
	score = bestScore;
	singularExtensionNode =
		!RootNode
		&& !SPNode
		&& 8 * OnePly <= depth
		&& !ttMove.IsNone()
		&& excludedMove.IsNone()
		&& (tte->GetType() & BoundLower)
		&& depth - 3 * OnePly <= tte->GetDepth();

	// step11
	// Loop through moves
	while (!(move = mp.GetNextMove<SPNode>()).IsNone()) {
		if (move == excludedMove) {
			continue;
		}

		if (RootNode
			&& std::find(m_rootMoves.begin() + m_pvIdx,
						 m_rootMoves.end(),
						 move) == m_rootMoves.end())
		{
			continue;
		}

		if (SPNode) {
			if (!pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
				continue;
			}
			moveCount = ++splitPoint->m_moveCount;
			splitPoint->m_mutex.unlock();
		}
		else {
			++moveCount;
		}


		if (RootNode) {
			m_signals.m_firstRootMove = (moveCount == 1);
#if 0
			if (GetThisThread == m_threads.GetMainThread() && 3000 < m_searchTimer.Elapsed()) {
				SYNCCOUT << "info depth " << GetDepth / OnePly
						 << " currmove " << GetMove.ToUSI()
						 << " currmovenumber " << m_moveCount + m_pvIdx << SYNCENDL;
			}
#endif
		}

		extension = Depth0;
		captureOrPawnPromotion = move.IsCaptureOrPawnPromotion();
		givesCheck = pos.IsMoveGivesCheck(move, ci);
		dangerous = givesCheck; // todo: not implement

		// step12
		if (givesCheck && ScoreZero <= pos.GetSeeSign(move))
		{
			extension = OnePly;
		}

		// singuler extension
		if (singularExtensionNode
			&& extension == Depth0
			&& move == ttMove
			&& pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)
			&& abs(ttScore) < g_ScoreKnownWin)
		{
			assert(ttScore != ScoreNone);

			const Score rBeta = ttScore - static_cast<Score>(depth);
			ss->m_excludedMove = move;
			ss->m_skipNullMove = true;
			score = Search<NonPV>(pos, ss, rBeta - 1, rBeta, depth / 2, cutNode);
			ss->m_skipNullMove = false;
			ss->m_excludedMove = Move::GetMoveNone();

			if (score < rBeta) {
				//extension = OnePly;
				extension = (beta <= rBeta ? OnePly + OnePly / 2 : OnePly);
			}
		}

		newDepth = depth - OnePly + extension;

		// step13
		// futility pruning
		if (!PVNode
			&& !captureOrPawnPromotion
			&& !inCheck
			&& !dangerous
			//&& move != ttMove // 次の行がtrueならこれもtrueなので条件から省く。
			&& ScoreMatedInMaxPly < bestScore)
		{
			assert(move != ttMove);
			// move count based pruning
			if (depth < 16 * OnePly
				&& g_futilityMoveCounts.m_futilityMoveCounts[depth] <= moveCount
				&& (threatMove.IsNone() || !refutes(pos, move, threatMove)))
			{
				if (SPNode) {
					splitPoint->m_mutex.lock();
				}
				continue;
			}

			// score based pruning
			const Depth predictedDepth = newDepth - g_reductions.reduction<PVNode>(depth, moveCount);
			// gain を 2倍にする。
			const Score futilityScore = ss->m_staticEval + g_futilityMargins.GetFutilityMargin(predictedDepth, moveCount)
				+ 2 * m_gains.GetValue(move.IsDrop(), UtilPiece::FromColorAndPieceType(pos.GetTurn(), move.GetPieceTypeFromOrDropped()), move.To());

			if (futilityScore < beta) {
				bestScore = std::max(bestScore, futilityScore);
				if (SPNode) {
					splitPoint->m_mutex.lock();
					if (splitPoint->m_bestScore < bestScore) {
						splitPoint->m_bestScore = bestScore;
					}
				}
				continue;
			}

			if (predictedDepth < 4 * OnePly
				&& pos.GetSeeSign(move) < ScoreZero)
			{
				if (SPNode) {
					splitPoint->m_mutex.lock();
				}
				continue;
			}
		}

		// RootNode, SPNode はすでに合法手であることを確認済み。
		if (!RootNode && !SPNode && !pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
			--moveCount;
			continue;
		}

		isPVMove = (PVNode && moveCount == 1);
		ss->m_currentMove = move;
		if (!SPNode && !captureOrPawnPromotion && playedMoveCount < 64) {
			movesSearched[playedMoveCount++] = move;
		}

		// step14
		pos.DoMove(move, st, ci, givesCheck);
		(ss+1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;

		// step15
		// LMR
		if (3 * OnePly <= depth
			&& !isPVMove
			&& !captureOrPawnPromotion
			&& move != ttMove
			&& ss->m_killers[0] != move
			&& ss->m_killers[1] != move)
		{
			ss->m_reduction = g_reductions.reduction<PVNode>(depth, moveCount);
			if (!PVNode && cutNode) {
				ss->m_reduction += OnePly;
			}
			const Depth d = std::max(newDepth - ss->m_reduction, OnePly);
			if (SPNode) {
				alpha = splitPoint->m_alpha;
			}
			// PVS
			score = -Search<NonPV>(pos, ss+1, -(alpha + 1), -alpha, d, true);

			doFullDepthSearch = (alpha < score && ss->m_reduction != Depth0);
			ss->m_reduction = Depth0;
		}
		else {
			doFullDepthSearch = !isPVMove;
		}

		// step16
		// full depth search
		// PVS
		if (doFullDepthSearch) {
			if (SPNode) {
				alpha = splitPoint->m_alpha;
			}
			score = (newDepth < OnePly ?
					 (givesCheck ? -Qsearch<NonPV, true>(pos, ss+1, -(alpha + 1), -alpha, Depth0)
					  : -Qsearch<NonPV, false>(pos, ss+1, -(alpha + 1), -alpha, Depth0))
					 : -Search<NonPV>(pos, ss+1, -(alpha + 1), -alpha, newDepth, !cutNode));
		}

		// 通常の探索
		if (PVNode && (isPVMove || (alpha < score && (RootNode || score < beta)))) {
			score = (newDepth < OnePly ?
					 (givesCheck ? -Qsearch<PV, true>(pos, ss+1, -beta, -alpha, Depth0)
					  : -Qsearch<PV, false>(pos, ss+1, -beta, -alpha, Depth0))
					 : -Search<PV>(pos, ss+1, -beta, -alpha, newDepth, false));
		}

		// step17
		pos.UndoMove(move);

		assert(-ScoreInfinite < score && score < ScoreInfinite);

		// step18
		if (SPNode) {
			splitPoint->m_mutex.lock();
			bestScore = splitPoint->m_bestScore;
			alpha = splitPoint->m_alpha;
		}

		if (m_signals.m_stop || thisThread->CutoffOccurred()) {
			return score;
		}

		if (RootNode) {
			RootMove& rm = *std::find(m_rootMoves.begin(), m_rootMoves.end(), move);
			if (isPVMove || alpha < score) {
				// PV move or new best move
				rm.m_score_ = score;
#if defined BISHOP_IN_DANGER
				if ((bishopInDangerFlag == BlackBishopInDangerIn28 && GetMove.ToCSA() == "0082KA")
					|| (bishopInDangerFlag == WhiteBishopInDangerIn28 && GetMove.ToCSA() == "0028KA")
					|| (bishopInDangerFlag == BlackBishopInDangerIn78 && GetMove.ToCSA() == "0032KA")
					|| (bishopInDangerFlag == WhiteBishopInDangerIn78 && GetMove.ToCSA() == "0078KA"))
				{
					rm.m_score_ -= m_engineOptions["Danger_Demerit_Score"];
				}
#endif
				rm.ExtractPvFromTT(pos);

				if (!isPVMove) {
					++m_bestMoveChanges;
				}
			}
			else {
				rm.m_score_ = -ScoreInfinite;
			}
		}

		if (bestScore < score) {
			bestScore = (SPNode ? splitPoint->m_bestScore = score : score);

			if (alpha < score) {
				bestMove = (SPNode ? splitPoint->m_bestMove = move : move);

				if (PVNode && score < beta) {
					alpha = (SPNode ? splitPoint->m_alpha = score : score);
				}
				else {
					// fail high
					if (SPNode) {
						splitPoint->m_cutoff = true;
					}
					break;
				}
			}
		}

		// step19
		if (!SPNode
			&& m_threads.GetMinSplitDepth() <= depth
			&& m_threads.GetAvailableSlave(thisThread)
			&& thisThread->m_splitPointsSize < g_MaxSplitPointsPerThread)
		{
			assert(bestScore < beta);
			thisThread->Split<FakeSplit>(pos, ss, alpha, beta, bestScore, bestMove,
										 depth, threatMove, moveCount, mp, NT, cutNode);
			if (beta <= bestScore) {
				break;
			}
		}
	}

	if (SPNode) {
		return bestScore;
	}

	// step20
	if (moveCount == 0) {
		return !excludedMove.IsNone() ? alpha : UtilScore::MatedIn(ss->m_ply);
	}

	if (bestScore == -ScoreInfinite) {
		assert(playedMoveCount == 0);
		bestScore = alpha;
	}

	if (beta <= bestScore) {
		// failed high
		m_tt.Store(posKey, scoreToTT(bestScore, ss->m_ply), BoundLower, depth,
				 bestMove, ss->m_staticEval);

		if (!bestMove.IsCaptureOrPawnPromotion() && !inCheck) {
			if (bestMove != ss->m_killers[0]) {
				ss->m_killers[1] = ss->m_killers[0];
				ss->m_killers[0] = bestMove;
			}

			const Score bonus = static_cast<Score>(depth * depth);
			const Piece pc1 = UtilPiece::FromColorAndPieceType(pos.GetTurn(), bestMove.GetPieceTypeFromOrDropped());
			m_history.Update(bestMove.IsDrop(), pc1, bestMove.To(), bonus);

			for (int i = 0; i < playedMoveCount - 1; ++i) {
				const Move m = movesSearched[i];
				const Piece pc2 = UtilPiece::FromColorAndPieceType(pos.GetTurn(), m.GetPieceTypeFromOrDropped());
				m_history.Update(m.IsDrop(), pc2, m.To(), -bonus);
			}
		}
	}
	else {
		// failed low or PV search
		m_tt.Store(posKey, scoreToTT(bestScore, ss->m_ply),
				 ((PVNode && !bestMove.IsNone()) ? BoundExact : BoundUpper),
				 depth, bestMove, ss->m_staticEval);
	}

	assert(-ScoreInfinite < bestScore && bestScore < ScoreInfinite);

	return bestScore;
}

void RootMove::ExtractPvFromTT(Position& pos) {
	StateInfo state[g_maxPlyPlus2];
	StateInfo* st = state;
	TTEntry* tte;
	Ply ply = 0;
	Move m = m_pv_[0];

	assert(!m.IsNone() && pos.MoveIsPseudoLegal(m));

	m_pv_.clear();

	do {
		m_pv_.push_back(m);

		assert(pos.MoveIsLegal(m_pv_[ply]));
		pos.DoMove(m_pv_[ply++], *st++);
		tte = pos.GetCsearcher()->m_tt.Probe(pos.GetKey());
	}
	while (tte != nullptr
		   // このチェックは少し無駄。駒打ちのときはmove16toMove() 呼ばなくて良い。
		   && pos.MoveIsPseudoLegal(m = UtilMoveStack::Move16toMove(tte->GetMove(), pos))
		   && pos.IsPseudoLegalMoveIsLegal<false, false>(m, pos.GetPinnedBB())
		   && ply < g_maxPly
		   && (!pos.IsDraw(20) || ply < 6));

	m_pv_.push_back(Move::GetMoveNone());
	while (ply) {
		pos.UndoMove(m_pv_[--ply]);
	}
}

void RootMove::InsertPvInTT(Position& pos) {
	StateInfo state[g_maxPlyPlus2];
	StateInfo* st = state;
	TTEntry* tte;
	Ply ply = 0;

	do {
		tte = pos.GetCsearcher()->m_tt.Probe(pos.GetKey());

		if (tte == nullptr
			|| UtilMoveStack::Move16toMove(tte->GetMove(), pos) != m_pv_[ply])
		{
			pos.GetSearcher()->m_tt.Store(pos.GetKey(), ScoreNone, BoundNone, DepthNone, m_pv_[ply], ScoreNone);
		}

		assert(pos.MoveIsLegal(m_pv_[ply]));
		pos.DoMove(m_pv_[ply++], *st++);
	} while (!m_pv_[ply].IsNone());

	while (ply) {
		pos.UndoMove(m_pv_[--ply]);
	}
}

void InitSearchTable() {
	// todo: パラメータは改善の余地あり。

	g_reductions.Initialize();

	g_futilityMargins.Initialize();

	// initOptions futility move counts
	//int iDepth;  // depth (ONE_PLY == 2)
	for (int iDepth = 0; iDepth < 32; ++iDepth) {
		g_futilityMoveCounts.m_futilityMoveCounts[iDepth] = static_cast<int>(3.001 + 0.3 * pow(static_cast<double>(iDepth), 1.8));
	}
}

// 入玉勝ちかどうかを判定
bool nyugyoku(const Position& pos) {
	// CSA ルールでは、一 から 六 の条件を全て満たすとき、入玉勝ち宣言が出来る。

	// 一 宣言側の手番である。

	// この関数を呼び出すのは自分の手番のみとする。ponder では呼び出さない。

	const Color us = pos.GetTurn();
	// 敵陣のマスク
	const Bitboard opponentsField = (us == Black ? g_inFrontMaskBb.GetInFrontMask<Black, Rank6>() : g_inFrontMaskBb.GetInFrontMask<White, Rank4>());

	// 二 宣言側の玉が敵陣三段目以内に入っている。
	if (!pos.GetBbOf(N08_King, us).AndIsNot0(opponentsField))
		return false;

	// 三 宣言側が、大駒5点小駒1点で計算して
	//     先手の場合28点以上の持点がある。
	//     後手の場合27点以上の持点がある。
	//     点数の対象となるのは、宣言側の持駒と敵陣三段目以内に存在する玉を除く宣言側の駒のみである。
	const Bitboard bigBB = pos.GetBbOf(N06_Rook, N14_Dragon, N05_Bishop, N13_Horse) & opponentsField & pos.GetBbOf(us);
	const Bitboard smallBB = (pos.GetBbOf(N01_Pawn, N02_Lance, N03_Knight, N04_Silver) | pos.GetGoldsBB()) & opponentsField & pos.GetBbOf(us);
	const Hand hand = pos.GetHand(us);
	const int val = (bigBB.PopCount() + hand.NumOf<HRook>() + hand.NumOf<HBishop>()) * 5
		+ smallBB.PopCount()
		+ hand.NumOf<HPawn>() + hand.NumOf<HLance>() + hand.NumOf<HKnight>()
		+ hand.NumOf<HSilver>() + hand.NumOf<HGold>();
#if defined LAW_24
	if (val < 31)
		return false;
#else
	if (val < (us == Black ? 28 : 27))
		return false;
#endif

	// 四 宣言側の敵陣三段目以内の駒は、玉を除いて10枚以上存在する。

	// 玉は敵陣にいるので、自駒が敵陣に11枚以上あればよい。
	if ((pos.GetBbOf(us) & opponentsField).PopCount() < 11)
		return false;

	// 五 宣言側の玉に王手がかかっていない。
	if (pos.InCheck())
		return false;

	// 六 宣言側の持ち時間が残っている。

	// 持ち時間が無ければ既に負けなので、何もチェックしない。

	return true;
}

void Searcher::Think() {
	static Book book;
	Position& pos = m_rootPosition;
	m_timeManager.Init(m_limits, pos.GetGamePly(), pos.GetTurn(), this);
	std::uniform_int_distribution<int> dist(this->m_engineOptions["Min_Book_Ply"], this->m_engineOptions["Max_Book_Ply"]);
	const Ply book_ply = dist(g_randomTimeSeed);

	bool nyugyokuWin = false;
#if defined LEARN
#else
	if (nyugyoku(pos)) {
		nyugyokuWin = true;
		goto finalize;
	}
#endif
	pos.SetNodesSearched(0);

#if defined LEARN
	m_threads[0]->m_searching = true;
#else
	m_tt.SetSize(m_engineOptions["USI_Hash"]); // operator int() 呼び出し。

	SYNCCOUT << "info string book_ply " << book_ply << SYNCENDL;
	if (m_engineOptions["OwnBook"] && pos.GetGamePly() <= book_ply) {
		const MoveScore bookMoveScore = book.GetProbe(pos, m_engineOptions["Book_File"], m_engineOptions["Best_Book_Move"]);
		if (
			!bookMoveScore.m_move.IsNone()
			&&
			std::find(
				m_rootMoves.begin(),
				m_rootMoves.end(),
				bookMoveScore.m_move
			) != m_rootMoves.end()
		)
		{
			std::swap(m_rootMoves[0], *std::find(m_rootMoves.begin(),
											   m_rootMoves.end(),
											   bookMoveScore.m_move));
			SYNCCOUT << "info"
					 << " score " << scoreToUSI(bookMoveScore.m_score)
					 << " pv " << bookMoveScore.m_move.ToUSI()
					 << SYNCENDL;

			goto finalize;
		}
	}

	Searcher::m_threads.WakeUp(this);

	Searcher::m_threads.GetTimerThread()->m_msec =
		(m_limits.IsUseTimeManagement() ? std::min(100, std::max(m_timeManager.AvailableTime() / 16, TimerResolution)) :
		 m_limits.m_nodes               ? 2 * TimerResolution :
		 100);
	Searcher::m_threads.GetTimerThread()->NotifyOne();

#if defined INANIWA_SHIFT
	detectInaniwa(GetPos);
#endif
#if defined BISHOP_IN_DANGER
	detectBishopInDanger(GetPos);
#endif
#endif
	IdLoop(pos);

#if defined LEARN
#else
	m_threads.GetTimerThread()->m_msec = 0; // timer を止める。
	m_threads.Sleep();

finalize:

	SYNCCOUT << "info nodes " << pos.GetNodesSearched()
			 << " time " << m_searchTimer.Elapsed() << SYNCENDL;

	if (!m_signals.m_stop && (m_limits.m_ponder || m_limits.m_infinite)) {
		m_signals.m_stopOnPonderHit = true;
		pos.GetThisThread()->WaitFor(m_signals.m_stop);
	}

	if (nyugyokuWin) {
		SYNCCOUT << "bestmove win" << SYNCENDL;
	}
	else if (m_rootMoves[0].m_pv_[0].IsNone()) {
		SYNCCOUT << "bestmove resign" << SYNCENDL;
	}
	else {
		SYNCCOUT << "bestmove " << m_rootMoves[0].m_pv_[0].ToUSI()
				 << " ponder " << m_rootMoves[0].m_pv_[1].ToUSI()
				 << SYNCENDL;
	}
#endif
}

void Searcher::CheckTime() {
	if (m_limits.m_ponder)
		return;

	s64 nodes = 0;
	if (m_limits.m_nodes) {
		std::unique_lock<Mutex> lock(m_threads.m_mutex_);

		nodes = m_rootPosition.GetNodesSearched();
		for (size_t i = 0; i < m_threads.size(); ++i) {
			for (int j = 0; j < m_threads[i]->m_splitPointsSize; ++j) {
				SplitPoint& splitPoint = m_threads[i]->m_SplitPoints[j];
				std::unique_lock<Mutex> spLock(splitPoint.m_mutex);
				nodes += splitPoint.m_nodes;
				u64 sm = splitPoint.m_slavesMask;
				while (sm) {
					const int index = firstOneFromLSB(sm);
					sm &= sm - 1;
					Position* pos = m_threads[index]->m_activePosition;
					if (pos != nullptr) {
						nodes += pos->GetNodesSearched();
					}
				}
			}
		}
	}

	const int e = m_searchTimer.Elapsed();
	const bool stillAtFirstMove =
		m_signals.m_firstRootMove
		&& !m_signals.m_failedLowAtRoot
		&& m_timeManager.AvailableTime() < e;

	const bool noMoreTime =
		m_timeManager.MaximumTime() - 2 * TimerResolution < e
		|| stillAtFirstMove;

	if ((m_limits.IsUseTimeManagement() && noMoreTime)
		|| (m_limits.m_moveTime != 0 && m_limits.m_moveTime < e)
		|| (m_limits.m_nodes != 0 && m_limits.m_nodes < nodes))
	{
		m_signals.m_stop = true;
	}
}

void Thread::IdleLoop() {
	SplitPoint* thisSp = m_splitPointsSize ? m_activeSplitPoint : nullptr;
	assert(!thisSp || (thisSp->m_masterThread == this && m_searching));

	while (true) {
		while ((!m_searching && m_pSearcher->m_threads.m_isSleepWhileIdle_) || m_exit)
		{
			if (m_exit) {
				assert(thisSp == nullptr);
				return;
			}

			std::unique_lock<Mutex> lock(m_sleepLock);
			if (thisSp != nullptr && !thisSp->m_slavesMask) {
				break;
			}

			if (!m_searching && !m_exit) {
				m_sleepCond.wait(lock);
			}
		}

		if (m_searching) {
			assert(!m_exit);

			m_pSearcher->m_threads.m_mutex_.lock();
			assert(m_searching);
			SplitPoint* sp = m_activeSplitPoint;
			m_pSearcher->m_threads.m_mutex_.unlock();

			SearchStack ss[g_maxPlyPlus2];
			Position pos(*sp->m_position, this);

			memcpy(ss, sp->m_searchStack - 1, 4 * sizeof(SearchStack));
			(ss+1)->m_splitPoint = sp;

			sp->m_mutex.lock();

			assert(m_activePosition == nullptr);

			m_activePosition = &pos;

			switch (sp->m_nodeType) {
			case Root : m_pSearcher->Search<SplitPointRoot >(pos, ss + 1, sp->m_alpha, sp->m_beta, sp->m_depth, sp->m_cutNode); break;
			case PV   : m_pSearcher->Search<SplitPointPV   >(pos, ss + 1, sp->m_alpha, sp->m_beta, sp->m_depth, sp->m_cutNode); break;
			case NonPV: m_pSearcher->Search<SplitPointNonPV>(pos, ss + 1, sp->m_alpha, sp->m_beta, sp->m_depth, sp->m_cutNode); break;
			default   : UNREACHABLE;
			}

			assert(m_searching);
			m_searching = false;
			m_activePosition = nullptr;
			assert(sp->m_slavesMask & (UINT64_C(1) << m_idx));
			sp->m_slavesMask ^= (UINT64_C(1) << m_idx);
			sp->m_nodes += pos.GetNodesSearched();

			if (m_pSearcher->m_threads.m_isSleepWhileIdle_
				&& this != sp->m_masterThread
				&& !sp->m_slavesMask)
			{
				assert(!sp->m_masterThread->m_searching);
				sp->m_masterThread->NotifyOne();
			}
			sp->m_mutex.unlock();
		}

		if (thisSp != nullptr && !thisSp->m_slavesMask) {
			thisSp->m_mutex.lock();
			const bool finished = !thisSp->m_slavesMask;
			thisSp->m_mutex.unlock();
			if (finished) {
				return;
			}
		}
	}
}



void Searcher::SetOption(std::istringstream& ssCmd) {
	std::string token;
	std::string name;
	std::string value;

	ssCmd >> token; // "name" が入力されるはず。

	ssCmd >> name;
	// " " が含まれた名前も扱う。
	while (ssCmd >> token && token != "value") {
		name += " " + token;
	}

	ssCmd >> value;
	// " " が含まれた値も扱う。
	while (ssCmd >> token) {
		value += " " + token;
	}

	if (!m_engineOptions.IsLegalOption(name)) {
		std::cout << "No such option: " << name << std::endl;
	}
	else {
		m_engineOptions[name] = value;
	}
}
