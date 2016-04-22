#include <iostream>
#include "../../header/n119_score___/n119_200_pieceScore.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_220_queenAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n800_learn___/n800_100_stopwatch.hpp"
#include "../../header/n886_repeType/n886_100_rtNot.hpp"
#include "../../header/n886_repeType/n886_110_rtDraw.hpp"
#include "../../header/n886_repeType/n886_120_rtWin.hpp"
#include "../../header/n886_repeType/n886_130_rtLose.hpp"
#include "../../header/n886_repeType/n886_140_rtSuperior.hpp"
#include "../../header/n886_repeType/n886_150_rtInferior.hpp"
#include "../../header/n886_repeType/n886_500_rtArray.hpp"
#include "../../header/n220_position/n220_100_repetitionType.hpp"
#include "../../header/n220_position/n220_640_utilAttack.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_665_utilMoveStack.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_040_nodeType.hpp"
#include "../../header/n223_move____/n223_300_moveAndScoreIndex.hpp"
#include "../../header/n350_pieceTyp/n350_500_ptPrograms.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../../header/n520_evaluate/n520_700_evaluation09.hpp"
#include "../../header/n560_timeMng_/n560_500_timeManager.hpp"
#include "../../header/n600_book____/n600_500_book.hpp"
#include "../../header/n640_searcher/n640_440_splitedNode.hpp"
#include "../../header/n640_searcher/n640_500_reductions.hpp"
#include "../../header/n640_searcher/n640_510_futilityMargins.hpp"
#include "../../header/n640_searcher/n640_520_futilityMoveCounts.hpp"
#include "../../header/n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../../header/n680_egOption/n680_300_engineOptionSetup.hpp"
#include "../../header/n760_thread__/n760_400_herosPub.hpp"
#include "../../header/n883_nodeType/n883_070_nodetypeAbstract.hpp"

#include "../../header/n885_searcher/n885_500_rucksack.hpp"
#include "../../header/n885_searcher/n885_600_iterativeDeepeningLoop.hpp"//FIXME:
#include "../../header/n887_nodeType/n887_500_nodetypePrograms.hpp"//FIXME:
//class IterativeDeepeningLoop;
//static inline void IterativeDeepeningLoop::Execute(Rucksack& rucksack, Position& pos);
#include "../../header/n885_searcher/n885_510_hitchhiker.hpp"

using namespace std;


extern const InFrontMaskBb g_inFrontMaskBb;
extern NodetypeAbstract* g_NODETYPE_PROGRAMS[];
extern RepetitionTypeArray g_repetitionTypeArray;




ScoreIndex Hitchhiker::Travel(
	Rucksack& rucksack,
	NodeType NT,
	Position& pos, Flashlight* ss, ScoreIndex alpha, ScoreIndex beta, const Depth depth, const bool cutNode
	) {
	NodetypeAbstract* nodetypeProgram = g_NODETYPE_PROGRAMS[NT];
	const bool PVNode = nodetypeProgram->IsPvNode();
	const bool SPNode = nodetypeProgram->IsSplitedNode();
	const bool RootNode = nodetypeProgram->IsRootNode();


	assert(-ScoreInfinite <= alpha && alpha < beta && beta <= ScoreInfinite);
	assert(PVNode || (alpha == beta - 1));
	assert(Depth0 < depth);

	// 途中で goto を使用している為、先に全部の変数を定義しておいた方が安全。
	Move movesSearched[64];
	StateInfo st;
	const TTEntry* tte;
	SplitedNode* splitedNode = nullptr;//(^q^)
	Key posKey;
	Move ttMove;
	Move move;
	Move excludedMove;
	Move bestMove;
	Move threatMove;
	Depth newDepth;
	Depth extension;
	ScoreIndex bestScore;
	ScoreIndex score;
	ScoreIndex ttScore;
	ScoreIndex eval;
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
	Military* thisThread = pos.GetThisThread();
	moveCount = playedMoveCount = 0;
	inCheck = pos.InCheck();

	if (SPNode) {
		splitedNode = ss->m_splitedNode;
		bestMove = splitedNode->m_bestMove;
		threatMove = splitedNode->m_threatMove;
		bestScore = splitedNode->m_bestScore;
		tte = nullptr;
		ttMove = excludedMove = g_MOVE_NONE;
		ttScore = ScoreNone;

		Evaluation09 evaluation;
		evaluation.evaluate(pos, ss);

		assert(-ScoreInfinite < splitedNode->m_bestScore && 0 < splitedNode->m_moveCount);

		goto split_point_start;
	}

	bestScore = -ScoreInfinite;
	ss->m_currentMove = threatMove = bestMove = (ss + 1)->m_excludedMove = g_MOVE_NONE;
	ss->m_ply = (ss - 1)->m_ply + 1;
	(ss + 1)->m_skipNullMove = false;
	(ss + 1)->m_reduction = Depth0;
	(ss + 2)->m_killers[0] = (ss + 2)->m_killers[1] = g_MOVE_NONE;

	if (PVNode && thisThread->m_maxPly < ss->m_ply) {
		thisThread->m_maxPly = ss->m_ply;
	}

	if (!RootNode) {
		// step2
		// stop と最大探索深さのチェック

		bool isReturn = false;
		ScoreIndex resultScore = ScoreIndex::ScoreNone;

		g_repetitionTypeArray.m_repetitionTypeArray[pos.IsDraw(16)]->CheckStopAndMaxPly(
			isReturn, resultScore, &rucksack, ss);

		if (isReturn)
		{
			return resultScore;
		}

		// step3
		// mate distance pruning
		if (!RootNode) {
			alpha = std::max(UtilScore::MatedIn(ss->m_ply), alpha);
			beta = std::min(UtilScore::MateIn(ss->m_ply + 1), beta);
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
	tte = rucksack.m_tt.Probe(posKey);
	ttMove =
		RootNode ? rucksack.m_rootMoves[rucksack.m_pvIdx].m_pv_[0] :
		tte != nullptr ?
		UtilMoveStack::Move16toMove(tte->GetMove(), pos) :
		g_MOVE_NONE;
	ttScore = (tte != nullptr ? rucksack.ConvertScoreFromTT(tte->GetScore(), ss->m_ply) : ScoreNone);

	if (!RootNode
		&& tte != nullptr
		&& depth <= tte->GetDepth()
		&& ttScore != ScoreNone // アクセス競合が起きたときのみ、ここに引っかかる。
		&& (PVNode ? tte->GetType() == BoundExact
			: (beta <= ttScore ? (tte->GetType() & BoundLower)
				: (tte->GetType() & BoundUpper))))
	{
		rucksack.m_tt.Refresh(tte);
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
			rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, ss->m_ply), BoundExact, depth,
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
		rucksack.m_tt.Store(posKey, ScoreNone, BoundNone, DepthNone,
			g_MOVE_NONE, ss->m_staticEval);
	}

	// 一手前の指し手について、history を更新する。
	// todo: ここの条件はもう少し考えた方が良い。
	if ((move = (ss - 1)->m_currentMove) != g_MOVE_NULL
		&& (ss - 1)->m_staticEval != ScoreNone
		&& ss->m_staticEval != ScoreNone
		&& !move.IsCaptureOrPawnPromotion() // 前回(一手前)の指し手が駒取りでなかった。
		)
	{
		const Square to = move.To();
		rucksack.m_gains.Update(move.IsDrop(), pos.GetPiece(to), to, -(ss - 1)->m_staticEval - ss->m_staticEval);
	}

	// step6
	// razoring
	if (!PVNode
		&& depth < 4 * OnePly
		&& eval + rucksack.razorMargin(depth) < beta
		&& ttMove.IsNone()
		&& abs(beta) < ScoreMateInMaxPly)
	{
		const ScoreIndex rbeta = beta - rucksack.razorMargin(depth);
		const ScoreIndex s = Hitchhiker::Qsearch(rucksack, N02_NonPV, false, pos, ss, rbeta - 1, rbeta, Depth0);
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
		ss->m_currentMove = g_MOVE_NULL;
		Depth reduction = static_cast<Depth>(3) * OnePly + depth / 4;

		if (beta < eval - PieceScore::m_pawn) {
			reduction += OnePly;
		}

		pos.DoNullMove(true, st);
		(ss + 1)->m_staticEvalRaw = (ss)->m_staticEvalRaw; // 評価値の差分評価の為。
		(ss + 1)->m_skipNullMove = true;
		ScoreIndex nullScore = (depth - reduction < OnePly ?
			-Hitchhiker::Qsearch(rucksack, N02_NonPV, false, pos, ss + 1, -beta, -alpha, Depth0)
			: -Hitchhiker::Travel(rucksack, NodeType::N02_NonPV, pos, ss + 1, -beta, -alpha, depth - reduction, !cutNode));
		(ss + 1)->m_skipNullMove = false;
		pos.DoNullMove(false, st);

		if (beta <= nullScore) {
			if (ScoreMateInMaxPly <= nullScore) {
				nullScore = beta;
			}

			if (depth < 6 * OnePly) {
				return nullScore;
			}

			ss->m_skipNullMove = true;
			assert(Depth0 < depth - reduction);
			const ScoreIndex s = Hitchhiker::Travel(rucksack, NodeType::N02_NonPV, pos, ss, alpha, beta, depth - reduction, false);
			ss->m_skipNullMove = false;

			if (beta <= s) {
				return nullScore;
			}
		}
		else {
			// fail low
			threatMove = (ss + 1)->m_currentMove;
			if (depth < 5 * OnePly
				&& (ss - 1)->m_reduction != Depth0
				&& !threatMove.IsNone()
				&& rucksack.allows(pos, (ss - 1)->m_currentMove, threatMove))
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
		const ScoreIndex rbeta = beta + 200;
		const Depth rdepth = depth - OnePly - 3 * OnePly;

		assert(OnePly <= rdepth);
		assert(!(ss - 1)->m_currentMove.IsNone());
		assert((ss - 1)->m_currentMove != g_MOVE_NULL);

		assert(move == (ss - 1)->m_currentMove);
		// move.cap() は前回(一手前)の指し手で取った駒の種類
		NextmoveEvent mp(pos, ttMove, rucksack.m_history, move.GetCap());
		const CheckInfo ci(pos);
		while (!(move = mp.GetNextMove(false)).IsNone()) {
			if (pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
				ss->m_currentMove = move;
				pos.DoMove(move, st, ci, pos.IsMoveGivesCheck(move, ci));
				(ss + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
				score = -Hitchhiker::Travel(rucksack, NodeType::N02_NonPV, pos, ss + 1, -rbeta, -rbeta + 1, rdepth, !cutNode);
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
		&& (PVNode || (!inCheck && beta <= ss->m_staticEval + static_cast<ScoreIndex>(256))))
	{
		//const Depth d = depth - 2 * OnePly - (PVNode ? Depth0 : depth / 4);
		const Depth d = (PVNode ? depth - 2 * OnePly : depth / 2);

		ss->m_skipNullMove = true;
		if (PVNode)
		{
			Hitchhiker::Travel(rucksack, NodeType::N01_PV, pos, ss, alpha, beta, d, true);
		}
		else
		{
			Hitchhiker::Travel(rucksack, NodeType::N02_NonPV, pos, ss, alpha, beta, d, true);
		}
		ss->m_skipNullMove = false;

		tte = rucksack.m_tt.Probe(posKey);
		ttMove = (tte != nullptr ?
			UtilMoveStack::Move16toMove(tte->GetMove(), pos) :
			g_MOVE_NONE);
	}

split_point_start:
	NextmoveEvent mp(pos, ttMove, depth, rucksack.m_history, ss, PVNode ? -ScoreInfinite : beta);
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
	while (!(move = mp.GetNextMove(SPNode)).IsNone()) {
		if (move == excludedMove) {
			continue;
		}

		if (RootNode
			&& std::find(rucksack.m_rootMoves.begin() + rucksack.m_pvIdx,
				rucksack.m_rootMoves.end(),
				move) == rucksack.m_rootMoves.end())
		{
			continue;
		}

		if (SPNode) {
			if (!pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
				continue;
			}
			moveCount = ++splitedNode->m_moveCount;
			splitedNode->m_mutex.unlock();
		}
		else {
			++moveCount;
		}


		if (RootNode) {
			rucksack.m_signals.m_firstRootMove = (moveCount == 1);
#if 0
			if (GetThisThread == rucksack.m_ownerHerosPub.GetFirstCaptain() && 3000 < rucksack.m_stopwatch.GetElapsed()) {
				SYNCCOUT << "info depth " << GetDepth / OnePly
					<< " currmove " << GetMove.ToUSI()
					<< " currmovenumber " << rucksack.m_moveCount + rucksack.m_pvIdx << SYNCENDL;
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
			&& abs(ttScore) < PieceScore::m_ScoreKnownWin)
		{
			assert(ttScore != ScoreNone);

			const ScoreIndex rBeta = ttScore - static_cast<ScoreIndex>(depth);
			ss->m_excludedMove = move;
			ss->m_skipNullMove = true;
			score = Hitchhiker::Travel(rucksack, N02_NonPV, pos, ss, rBeta - 1, rBeta, depth / 2, cutNode);
			ss->m_skipNullMove = false;
			ss->m_excludedMove = g_MOVE_NONE;

			if (score < rBeta) {
				//extension = OnePly;
				extension = (beta <= rBeta ? OnePly + OnePly / 2 : OnePly);
			}
		}

		newDepth = depth - OnePly + extension;

		// step13
		bool isContinue = false;
		nodetypeProgram->DoStep13(
			isContinue,
			rucksack,
			captureOrPawnPromotion,
			inCheck,
			dangerous,
			bestScore,
			move,
			ttMove,
			depth,
			moveCount,
			threatMove,
			pos,
			splitedNode,
			newDepth,
			*ss,
			beta,
			ci,
			isPVMove,
			playedMoveCount,
			movesSearched
			);
		if (isContinue)
		{
			continue;
		}

		// step14
		pos.DoMove(move, st, ci, givesCheck);
		(ss + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;

		// step15
		// LMR
		if (3 * OnePly <= depth
			&& !isPVMove
			&& !captureOrPawnPromotion
			&& move != ttMove
			&& ss->m_killers[0] != move
			&& ss->m_killers[1] != move)
		{
			ss->m_reduction = g_reductions.reduction(PVNode, depth, moveCount);
			if (!PVNode && cutNode) {
				ss->m_reduction += OnePly;
			}
			const Depth d = std::max(newDepth - ss->m_reduction, OnePly);
			if (SPNode) {
				alpha = splitedNode->m_alpha;
			}
			// PVS
			score = -Hitchhiker::Travel(
				rucksack, N02_NonPV, pos, ss + 1, -(alpha + 1), -alpha, d, true);

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
				alpha = splitedNode->m_alpha;
			}
			score = (newDepth < OnePly ?
				(givesCheck ? -Hitchhiker::Qsearch(rucksack, N02_NonPV, true, pos, ss + 1, -(alpha + 1), -alpha, Depth0)
					: -Hitchhiker::Qsearch(rucksack, N02_NonPV, false, pos, ss + 1, -(alpha + 1), -alpha, Depth0))
				: -Hitchhiker::Travel(
					rucksack, N02_NonPV, pos, ss + 1, -(alpha + 1), -alpha, newDepth, !cutNode));
		}

		// 通常の探索
		if (PVNode && (isPVMove || (alpha < score && (RootNode || score < beta)))) {
			score = (newDepth < OnePly ?
				(givesCheck ? -Hitchhiker::Qsearch(rucksack, N01_PV, true, pos, ss + 1, -beta, -alpha, Depth0)
					: -Hitchhiker::Qsearch(rucksack, N01_PV, false, pos, ss + 1, -beta, -alpha, Depth0))
				: -Hitchhiker::Travel(
					rucksack, N01_PV, pos, ss + 1, -beta, -alpha, newDepth, false));
		}

		// step17
		pos.UndoMove(move);

		assert(-ScoreInfinite < score && score < ScoreInfinite);

		// step18
		if (SPNode) {
			splitedNode->m_mutex.lock();
			bestScore = splitedNode->m_bestScore;
			alpha = splitedNode->m_alpha;
		}

		if (rucksack.m_signals.m_stop || thisThread->CutoffOccurred()) {
			return score;
		}

		bool isBreak = false;
		nodetypeProgram->DoStep18(
			isBreak,
			rucksack,
			move,
			isPVMove,
			alpha,
			score,
			pos,
			bestScore,
			splitedNode,
			bestMove,
			beta
			);
		if (isBreak) {
			break;
		}

		// step19
		nodetypeProgram->DoStep19(
			isBreak,
			rucksack,
			depth,
			thisThread,
			bestScore,
			beta,
			pos,
			*ss,
			alpha,
			bestMove,
			threatMove,
			moveCount,
			mp,
			NT,
			cutNode
			);
		if (isBreak) {
			break;
		}
	}

	if (SPNode) {
		return bestScore;
	}

	// step20
	if (moveCount == 0) {
		return !excludedMove.IsNone() ? alpha : UtilScore::MatedIn(ss->m_ply);
	}
	nodetypeProgram->DoStep20(
		rucksack,
		alpha,
		*ss,
		bestScore,
		playedMoveCount,
		beta,
		posKey,
		depth,
		bestMove,
		inCheck,
		pos,
		movesSearched
		);

	assert(-ScoreInfinite < bestScore && bestScore < ScoreInfinite);

	return bestScore;
}


ScoreIndex Hitchhiker::Qsearch(Rucksack& rucksack, NodeType NT, bool INCHECK,
	Position& pos, Flashlight* ss, ScoreIndex alpha, ScoreIndex beta, const Depth depth) {
	const bool PVNode = (NT == N01_PV);

	assert(NT == N01_PV || NT == N02_NonPV);
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
	ScoreIndex bestScore;
	ScoreIndex score;
	ScoreIndex ttScore;
	ScoreIndex futilityScore;
	ScoreIndex futilityBase;
	ScoreIndex oldAlpha;
	bool givesCheck;
	bool evasionPrunable;
	Depth ttDepth;

	if (PVNode) {
		oldAlpha = alpha;
	}

	ss->m_currentMove = bestMove = g_MOVE_NONE;
	ss->m_ply = (ss - 1)->m_ply + 1;

	if (g_maxPly < ss->m_ply) {
		return ScoreDraw;
	}

	ttDepth = ((INCHECK || DepthQChecks <= depth) ? DepthQChecks : DepthQNoChecks);

	posKey = pos.GetKey();
	tte = rucksack.m_tt.Probe(posKey);
	ttMove = (tte != nullptr ? UtilMoveStack::Move16toMove(tte->GetMove(), pos) : g_MOVE_NONE);
	ttScore = (tte != nullptr ? rucksack.ConvertScoreFromTT(tte->GetScore(), ss->m_ply) : ScoreNone);

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
			if (
				(ss->m_staticEval = bestScore = tte->GetEvalScore()) == ScoreNone
				) {
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
				rucksack.m_tt.Store(pos.GetKey(), rucksack.ConvertScoreToTT(bestScore, ss->m_ply), BoundLower,
					DepthNone, g_MOVE_NONE, ss->m_staticEval);
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

	NextmoveEvent mp(pos, ttMove, depth, rucksack.m_history, (ss - 1)->m_currentMove.To());
	const CheckInfo ci(pos);

	while (!(move = mp.GetNextMove(false)).IsNone())
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
				futilityBase + PieceScore::GetCapturePieceScore(pos.GetPiece(move.To()));
			if (move.IsPromotion()) {
				futilityScore += PieceScore::GetPromotePieceScore(move.GetPieceTypeFrom());
			}

			if (futilityScore < beta) {
				bestScore = std::max(bestScore, futilityScore);
				continue;
			}

			// todo: ＭｏｖｅＰｉｃｋｅｒ のオーダリングで SEE してるので、ここで SEE するの勿体無い。
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
		(ss + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
		score = (givesCheck ? -Hitchhiker::Qsearch(rucksack, NT, true, pos, ss + 1, -beta, -alpha, depth - OnePly)
			: -Hitchhiker::Qsearch(rucksack, NT, false, pos, ss + 1, -beta, -alpha, depth - OnePly));
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
					rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(score, ss->m_ply), BoundLower,
						ttDepth, move, ss->m_staticEval);
					return score;
				}
			}
		}
	}

	if (INCHECK && bestScore == -ScoreInfinite) {
		return UtilScore::MatedIn(ss->m_ply);
	}

	rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, ss->m_ply),
		((PVNode && oldAlpha < bestScore) ? BoundExact : BoundUpper),
		ttDepth, bestMove, ss->m_staticEval);

	assert(-ScoreInfinite < bestScore && bestScore < ScoreInfinite);

	return bestScore;
}



void Hitchhiker::Think(Rucksack& rucksack) {
	static Book book;
	Position& pos = rucksack.m_rootPosition;
	rucksack.m_timeManager.Init(rucksack.m_limits, pos.GetGamePly(), pos.GetTurn(), &rucksack);
	std::uniform_int_distribution<int> dist(rucksack.m_engineOptions["Min_Book_Ply"], rucksack.m_engineOptions["Max_Book_Ply"]);
	const Ply book_ply = dist(g_randomTimeSeed);

	bool nyugyokuWin = false;
#if defined LEARN
#else
	if (Hitchhiker::Nyugyoku(pos)) {
		nyugyokuWin = true;
		goto finalize;
	}
#endif
	pos.SetNodesSearched(0);

#if defined LEARN
	rucksack.m_ownerHerosPub[0]->m_searching = true;
#else
	rucksack.m_tt.SetSize(rucksack.m_engineOptions["USI_Hash"]); // operator int() 呼び出し。

	SYNCCOUT << "info string book_ply " << book_ply << SYNCENDL;
	if (rucksack.m_engineOptions["OwnBook"] && pos.GetGamePly() <= book_ply) {
		const MoveAndScoreIndex bookMoveScore = book.GetProbe(pos, rucksack.m_engineOptions["Book_File"], rucksack.m_engineOptions["Best_Book_Move"]);
		if (
			!bookMoveScore.m_move.IsNone()
			&&
			std::find(
				rucksack.m_rootMoves.begin(),
				rucksack.m_rootMoves.end(),
				bookMoveScore.m_move
				) != rucksack.m_rootMoves.end()
			)
		{
			std::swap(rucksack.m_rootMoves[0], *std::find(rucksack.m_rootMoves.begin(),
				rucksack.m_rootMoves.end(),
				bookMoveScore.m_move));
			SYNCCOUT << "info"
				<< " score " << rucksack.scoreToUSI(bookMoveScore.m_scoreIndex)
				<< " pv " << bookMoveScore.m_move.ToUSI()
				<< SYNCENDL;

			goto finalize;
		}
	}

	rucksack.m_ownerHerosPub.WakeUp(&rucksack);

	rucksack.m_ownerHerosPub.GetCurrWarrior()->m_msec =
		(rucksack.m_limits.IsUseTimeManagement() ?
			std::min(100, std::max(rucksack.m_timeManager.GetAvailableTime() / 16, rucksack.TimerResolution)) :
			rucksack.m_limits.m_nodes ?
			2 * rucksack.TimerResolution :
			100
			);

	rucksack.m_ownerHerosPub.GetCurrWarrior()->NotifyOne();

#if defined INANIWA_SHIFT
	detectInaniwa(GetPos);
#endif
#if defined BISHOP_IN_DANGER
	detectBishopInDanger(GetPos);
#endif
#endif
	// 反復深化探索を始めるぜ☆（＾ｑ＾）
	IterativeDeepeningLoop::Execute885_500(rucksack, pos);//ExecuteIterativeDeepeningLoop(pos);

#if defined LEARN
#else
	rucksack.m_ownerHerosPub.GetCurrWarrior()->m_msec = 0; // timer を止める。
	rucksack.m_ownerHerosPub.Sleep();

finalize:

	SYNCCOUT << "info nodes " << pos.GetNodesSearched()
		<< " time " << rucksack.m_stopwatch.GetElapsed() << SYNCENDL;

	if (!rucksack.m_signals.m_stop && (rucksack.m_limits.m_ponder || rucksack.m_limits.m_infinite)) {
		rucksack.m_signals.m_stopOnPonderHit = true;
		pos.GetThisThread()->WaitFor(rucksack.m_signals.m_stop);
	}

	if (nyugyokuWin) {
		SYNCCOUT << "bestmove win" << SYNCENDL;
	}
	else if (rucksack.m_rootMoves[0].m_pv_[0].IsNone()) {
		SYNCCOUT << "bestmove resign" << SYNCENDL;
	}
	else {
		SYNCCOUT << "bestmove " << rucksack.m_rootMoves[0].m_pv_[0].ToUSI()
			<< " ponder " << rucksack.m_rootMoves[0].m_pv_[1].ToUSI()
			<< SYNCENDL;
	}
#endif
}



// 入玉勝ちかどうかを判定
bool Hitchhiker::Nyugyoku(const Position& pos) {
	// CSA ルールでは、一 から 六 の条件を全て満たすとき、入玉勝ち宣言が出来る。

	// 一 宣言側の手番である。

	// この関数を呼び出すのは自分の手番のみとする。ponder では呼び出さない。

	const Color us = pos.GetTurn();
	// 敵陣のマスク
	const Bitboard opponentsField = (us == Black ? g_inFrontMaskBb.GetInFrontMask(Black, Rank6) : g_inFrontMaskBb.GetInFrontMask(White, Rank4));

	// 二 宣言側の玉が敵陣三段目以内に入っている。
	if (!pos.GetBbOf20(N08_King, us).AndIsNot0(opponentsField))
		return false;

	// 三 宣言側が、大駒5点小駒1点で計算して
	//     先手の場合28点以上の持点がある。
	//     後手の場合27点以上の持点がある。
	//     点数の対象となるのは、宣言側の持駒と敵陣三段目以内に存在する玉を除く宣言側の駒のみである。
	const Bitboard bigBB = pos.GetBbOf(N06_Rook, N14_Dragon, N05_Bishop, N13_Horse) & opponentsField & pos.GetBbOf10(us);
	const Bitboard smallBB = (pos.GetBbOf(N01_Pawn, N02_Lance, N03_Knight, N04_Silver) |
		pos.GetGoldsBB()) & opponentsField & pos.GetBbOf10(us);
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
	if ((pos.GetBbOf10(us) & opponentsField).PopCount() < 11)
		return false;

	// 五 宣言側の玉に王手がかかっていない。
	if (pos.InCheck())
		return false;

	// 六 宣言側の持ち時間が残っている。

	// 持ち時間が無ければ既に負けなので、何もチェックしない。

	return true;
}
