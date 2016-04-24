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

#include "../../header/n885_searcher/n885_040_rucksack.hpp"
#include "../../header/n885_searcher/n885_480_hitchhikerNyugyoku.hpp"
#include "../../header/n885_searcher/n885_600_iterativeDeepeningLoop.hpp"//FIXME:
#include "../../header/n887_nodeType/n887_500_nodetypePrograms.hpp"//FIXME:
//class IterativeDeepeningLoop;
//static inline void IterativeDeepeningLoop::Execute(Rucksack& rucksack, Position& pos);
#include "../../header/n885_searcher/n885_510_hitchhiker.hpp"

using namespace std;


extern const InFrontMaskBb g_inFrontMaskBb;
extern NodetypeAbstract* g_NODETYPE_PROGRAMS[];
extern RepetitionTypeArray g_repetitionTypeArray;




ScoreIndex Hitchhiker::Travel_885_510(
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
	const TTEntry* tte = nullptr;//(^q^)
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

	/*
	bool isGotoSplitPointStart = false;
	nodetypeProgram->DoStep1(
		isGotoSplitPointStart,
		moveCount,
		playedMoveCount,
		inCheck,
		pos,
		splitedNode,
		ss,
		bestMove,
		threatMove,
		bestScore,
		ttMove,
		excludedMove,
		ttScore,
		thisThread
		);
	if (isGotoSplitPointStart)
	{
		goto split_point_start;
	}
	//*/

	//*
	// initialize node
	moveCount = playedMoveCount = 0;
	inCheck = pos.InCheck();

	if (SPNode) {
		splitedNode = ss->m_splitedNode;
		bestMove = splitedNode->m_bestMove;
		threatMove = splitedNode->m_threatMove;
		bestScore = splitedNode->m_bestScore;
		//tte = nullptr;
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
	//*/

	bool isReturnWithScore = false;
	ScoreIndex returnScore = ScoreIndex::ScoreNone;
	if (!RootNode) {
		// step2
		// stop と最大探索深さのチェック

		nodetypeProgram->DoStep2(
			isReturnWithScore,
			returnScore,
			pos,
			rucksack,
			ss
		);
		/*
		g_repetitionTypeArray.m_repetitionTypeArray[pos.IsDraw(16)]->CheckStopAndMaxPly(
			isReturnWithScore, returnScore, &rucksack, ss);
		*/

		if (isReturnWithScore)
		{
			return returnScore;
		}

		// step3
		nodetypeProgram->DoStep3(
			isReturnWithScore,
			returnScore,
			ss,
			alpha,
			beta
			);
		if (isReturnWithScore)
		{
			return returnScore;
		}
		/*
		// mate distance pruning
		if (!RootNode) {
			alpha = std::max(UtilScore::MatedIn(ss->m_ply), alpha);
			beta = std::min(UtilScore::MateIn(ss->m_ply + 1), beta);
			if (beta <= alpha) {
				return alpha;
			}
		}
		*/
	}

	pos.SetNodesSearched(pos.GetNodesSearched() + 1);

	// step4
	nodetypeProgram->DoStep4(
		isReturnWithScore,
		returnScore,
		rucksack,
		excludedMove,
		ss,
		posKey,
		pos,
		tte,
		ttMove,
		ttScore,
		depth,
		beta,
		inCheck,
		move,
		bestScore,
		bestMove
		);
	if (isReturnWithScore)
	{
		return returnScore;
	}

	// step5
	bool isGotoIidStart = false;
	nodetypeProgram->DoStep5(
		isGotoIidStart,
		rucksack,
		eval,
		ss,
		pos,
		inCheck,
		tte,
		ttScore,
		posKey,
		move
		);
	if (isGotoIidStart) {
		goto iid_start;
	}

	// step6
	nodetypeProgram->DoStep6(
		isReturnWithScore,
		returnScore,
		rucksack,
		depth,
		eval,
		beta,
		ttMove,
		pos,
		ss
		);
	if (isReturnWithScore)
	{
		return returnScore;
	}

	// step7
	nodetypeProgram->DoStep7(
		isReturnWithScore,
		returnScore,
		ss,
		depth,
		beta,
		eval
		);

	// step8
	nodetypeProgram->DoStep8(
		isReturnWithScore,
		returnScore,
		rucksack,
		ss,
		depth,
		beta,
		eval,
		pos,
		st,
		alpha,
		cutNode,
		threatMove
	);
	if (isReturnWithScore) {
		return returnScore;
	}

	// step9
	nodetypeProgram->DoStep9(
		isReturnWithScore,
		rucksack,
		depth,
		ss,
		beta,
		move,
		pos,
		ttMove,
		st,
		score,
		cutNode
		);
	if (isReturnWithScore) {
		return score;
	}

	// 内側の反復深化探索☆？（＾ｑ＾）
iid_start:
	// step10
	nodetypeProgram->DoStep10(
		depth,
		ttMove,
		inCheck,
		beta,
		ss,
		rucksack,
		pos,
		alpha,
		tte,
		posKey
		);

split_point_start:
	NextmoveEvent mp(pos, ttMove, depth, rucksack.m_history, ss, PVNode ? -ScoreInfinite : beta);
	const CheckInfo ci(pos);

	nodetypeProgram->DoStep11A_BeforeLoop_SplitPointStart(
		ttMove,
		depth,
		score,
		bestScore,
		singularExtensionNode,
		excludedMove,
		tte
		);

	// step11
	// Loop through moves
	while (!(move = mp.GetNextMove(SPNode)).IsNone()) {

		bool isContinue = false;
		nodetypeProgram->DoStep11B_LoopHeader(
			rucksack,
			isContinue,
			move,
			excludedMove,
			pos,
			ci,
			moveCount,
			splitedNode,
			extension,
			captureOrPawnPromotion,
			givesCheck,
			dangerous
			);
		if (isContinue)
		{
			continue;
		}

		// step12
		nodetypeProgram->DoStep12(
			rucksack,
			givesCheck,
			pos,
			move,
			extension,
			singularExtensionNode,
			ttMove,
			ttScore,
			ci,
			depth,
			ss,
			score,
			cutNode,
			beta,
			newDepth
			);

		// step13
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
		nodetypeProgram->DoStep14(
			pos,
			move,
			st,
			ci,
			givesCheck,
			ss
			);

		// step15
		nodetypeProgram->DoStep15(
			rucksack,
			depth,
			isPVMove,
			captureOrPawnPromotion,
			move,
			ttMove,
			ss,
			PVNode,
			moveCount,
			cutNode,
			newDepth,
			alpha,
			splitedNode,
			score,
			pos,
			doFullDepthSearch
			);

		// step16
		nodetypeProgram->DoStep16(
			rucksack,
			doFullDepthSearch,
			splitedNode,
			alpha,
			score,
			newDepth,
			givesCheck,
			pos,
			ss,
			cutNode,
			isPVMove,
			beta
			);

		// step17
		nodetypeProgram->DoStep17(
			pos,
			move
			);

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

	Position& pos = rucksack.m_rootPosition;
	rucksack.m_timeManager.Init(rucksack.m_limits, pos.GetGamePly(), pos.GetTurn(), &rucksack);

	//────────────────────────────────────────────────────────────────────────────────
	// 入玉勝ちかを調べるぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
#if !defined(LEARN)
	if (Hitchhiker::IsNyugyokuWin(pos)) {
		//────────────────────────────────────────────────────────────────────────────────
		// 探索☆？（＾ｑ＾）
		//────────────────────────────────────────────────────────────────────────────────
		HitchhikerNyugyoku::Travel_885_480(rucksack,pos);
		return;
	}
#endif

	//────────────────────────────────────────────────────────────────────────────────
	// 入玉勝ちじゃなかったぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	static Book book;
	std::uniform_int_distribution<int> dist(
		rucksack.m_engineOptions["Min_Book_Ply"], rucksack.m_engineOptions["Max_Book_Ply"]);

	const Ply book_ply = dist(g_randomTimeSeed);

	pos.SetNodesSearched(0);

#if defined LEARN
	rucksack.m_ownerHerosPub[0]->m_searching = true;
#else
	rucksack.m_tt.SetSize(rucksack.m_engineOptions["USI_Hash"]); // operator int() 呼び出し。

	SYNCCOUT << "info string book_ply " << book_ply << SYNCENDL;
	if (
		// 定跡が使える手数のとき☆（＾ｑ＾）？
		rucksack.m_engineOptions["OwnBook"] &&
		pos.GetGamePly() <= book_ply
	) {
		const MoveAndScoreIndex bookMoveScore = book.GetProbe(
			pos, rucksack.m_engineOptions["Book_File"], rucksack.m_engineOptions["Best_Book_Move"]);

		if (
			!bookMoveScore.m_move.IsNone()
			&&
			std::find(
				rucksack.m_rootMoves.begin(),
				rucksack.m_rootMoves.end(),
				bookMoveScore.m_move
			) != rucksack.m_rootMoves.end()
		){
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

	// 全スレッドの初期化か何か☆？
	rucksack.m_ownerHerosPub.WakeUp(&rucksack);

	// 下級戦士の寿命（ミリ秒）を設定するぜ☆
	rucksack.m_ownerHerosPub.GetCurrWarrior()->m_lifetimeMilliseconds =
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
	//────────────────────────────────────────────────────────────────────────────────
	// 反復深化探索を始めるぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	IterativeDeepeningLoop::Execute885_500(rucksack, pos);//ExecuteIterativeDeepeningLoop(pos);

#if defined LEARN
#else
	rucksack.m_ownerHerosPub.GetCurrWarrior()->m_lifetimeMilliseconds = 0; // timer を止める。
	rucksack.m_ownerHerosPub.Sleep();

finalize:

	SYNCCOUT << "info nodes " << pos.GetNodesSearched()
		<< " time " << rucksack.m_stopwatch.GetElapsed() << SYNCENDL;

	if (!rucksack.m_signals.m_stop && (rucksack.m_limits.m_ponder || rucksack.m_limits.m_infinite)) {
		rucksack.m_signals.m_stopOnPonderHit = true;
		pos.GetThisThread()->WaitFor(rucksack.m_signals.m_stop);
	}

	if (rucksack.m_rootMoves[0].m_pv_[0].IsNone()) {
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
bool Hitchhiker::IsNyugyokuWin(const Position& pos) {
	// CSA ルールでは、一 から 六 の条件を全て満たすとき、入玉勝ち宣言が出来る。

	// 一 宣言側の手番である。

	// この関数を呼び出すのは自分の手番のみとする。ponder では呼び出さない。

	const Color us = pos.GetTurn();

	// 敵陣のマスク
	const Bitboard opponentsField = (us == Black ? g_inFrontMaskBb.GetInFrontMask(Black, Rank6) : g_inFrontMaskBb.GetInFrontMask(White, Rank4));

	// 二 宣言側の玉が敵陣三段目以内に入っている。
	if (!pos.GetBbOf20(N08_King, us).AndIsNot0(opponentsField))
	{
		return false;
	}

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
	{
		return false;
	}
#else
	if (val < (us == Black ? 28 : 27))
	{
		return false;
	}
#endif

	// 四 宣言側の敵陣三段目以内の駒は、玉を除いて10枚以上存在する。

	// 玉は敵陣にいるので、自駒が敵陣に11枚以上あればよい。
	if ((pos.GetBbOf10(us) & opponentsField).PopCount() < 11)
	{
		return false;
	}

	// 五 宣言側の玉に王手がかかっていない。
	if (pos.InCheck())
	{
		return false;
	}

	// 六 宣言側の持ち時間が残っている。

	// 持ち時間が無ければ既に負けていて　ここにはこないので、何もチェックしない（＾ｑ＾）

	return true;
}
