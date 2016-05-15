#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_665_utilMove01.hpp"
#include "../../header/n223_move____/n223_040_nodeType.hpp"
#include "../../header/n223_move____/n223_200_depth.hpp"
#include "../../header/n223_move____/n223_500_flashlight.hpp"

#include "../../header/n830_evaluate/n830_700_evaluation09.hpp"	// FIXME:
#include "../../header/n885_searcher/n885_040_rucksack.hpp"	// FIXME:
#include "../../header/n885_searcher/n885_310_hitchhikerQsearchAbstract.hpp"


// N01_PV か、N02_NonPV でだけ使うことができるぜ☆（＾ｑ＾）ルートでは使えない☆
// スプリット・ポイントかそうでないかは見てないぜ☆
// 静止探索☆ 再帰するぜ☆
ScoreIndex HitchhikerQsearchAbstract::DoQsearch(
	Rucksack& rucksack,
	bool INCHECK,
	Position& pos,
	Flashlight* pFlashlight,
	ScoreIndex alpha,
	ScoreIndex beta,
	const Depth depth
	) const {

	assert(INCHECK == pos.InCheck());
	assert(-ScoreInfinite <= alpha && alpha < beta && beta <= ScoreInfinite);
	this->DoAssert(alpha,beta);
	assert(depth <= Depth0);

	StateInfo st;
	const TTEntry* pTtEntry;
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

	this->SetOldAlpha(oldAlpha, alpha);

	pFlashlight->m_currentMove = bestMove = g_MOVE_NONE;
	pFlashlight->m_ply = (pFlashlight - 1)->m_ply + 1;

	if (g_maxPly < pFlashlight->m_ply) {
		return ScoreDraw;
	}

	ttDepth = ((INCHECK || DepthQChecks <= depth) ? DepthQChecks : DepthQNoChecks);

	posKey = pos.GetKey();
	pTtEntry = rucksack.m_tt.Probe(posKey);
	ttMove = (pTtEntry != nullptr ? UtilMove01::Move16toMove(pTtEntry->GetMove(), pos) : g_MOVE_NONE);
	ttScore = (pTtEntry != nullptr ? rucksack.ConvertScoreFromTT(pTtEntry->GetScore(), pFlashlight->m_ply) : ScoreNone);

	if (pTtEntry != nullptr
		&& ttDepth <= pTtEntry->GetDepth()
		&& ttScore != ScoreNone // アクセス競合が起きたときのみ、ここに引っかかる。
		&& this->GetCondition01(
			&pTtEntry,
			beta,
			ttScore
			)
	){
		pFlashlight->m_currentMove = ttMove;
		return ttScore;
	}

	pos.SetNodesSearched(pos.GetNodesSearched() + 1);

	if (INCHECK) {
		pFlashlight->m_staticEval = ScoreNone;
		bestScore = futilityBase = -ScoreInfinite;
	}
	else {
		if (!(move =
			(
				pos.GetTurn()==Color::Black
				?
				pos.GetMateMoveIn1Ply<Color::Black,Color::White>()
				:
				pos.GetMateMoveIn1Ply<Color::White, Color::Black>()
			)			
			).IsNone()) {
			return UtilScore::MateIn(pFlashlight->m_ply);
		}

		if (pTtEntry != nullptr) {
			if (
				(pFlashlight->m_staticEval = bestScore = pTtEntry->GetEvalScore()) == ScoreNone
				) {
				Evaluation09 evaluation;
				pFlashlight->m_staticEval = bestScore = evaluation.evaluate(pos, pFlashlight);
			}
		}
		else {
			Evaluation09 evaluation;
			pFlashlight->m_staticEval = bestScore = evaluation.evaluate(pos, pFlashlight);
		}

		if (beta <= bestScore) {
			if (pTtEntry == nullptr) {
				rucksack.m_tt.Store(pos.GetKey(), rucksack.ConvertScoreToTT(bestScore, pFlashlight->m_ply), BoundLower,
					DepthNone, g_MOVE_NONE, pFlashlight->m_staticEval);
			}

			return bestScore;
		}

		// PVノードのとき☆（＾ｑ＾）
		this->SetAlpha(alpha, bestScore);

		futilityBase = bestScore + 128; // todo: 128 より大きくて良いと思う。
	}

	Evaluation09 evaluation;
	evaluation.evaluate(pos, pFlashlight);

	NextmoveEvent videodeck( // 静止探索用
		pos,
		ttMove,
		depth,
		rucksack.m_history,
		(pFlashlight - 1)->m_currentMove.To()
	);
	const CheckInfo ci(pos);

	while (!(move = videodeck.GetNextMove_NonSplitedNode()).IsNone())
	{
		assert(pos.IsOK());

		givesCheck = pos.IsMoveGivesCheck(move, ci);

		// futility pruning
		// 非PVノードのとき☆（＾ｑ＾）
		bool isContinue = false;
		this->DoFutilityPruning01(
			isContinue,
			INCHECK,
			givesCheck,
			move,
			ttMove,
			futilityScore,
			futilityBase,
			pos,
			beta,
			bestScore,
			depth
			);
		if (isContinue) {
			continue;
		}

		evasionPrunable = (INCHECK
			&& ScoreMatedInMaxPly < bestScore
			&& !move.IsCaptureOrPawnPromotion());

		// 非PVノードのとき☆（＾ｑ＾）
		this->DoContinue01(
			isContinue,
			INCHECK,
			evasionPrunable,
			move,
			ttMove,
			pos
			);
		if (isContinue) {
			continue;
		}

		if (!
			(
				pos.GetTurn()==Color::Black
				?
				pos.IsPseudoLegalMoveIsLegal<false, false, Color::Black,Color::White>(move, ci.m_pinned)
				:
				pos.IsPseudoLegalMoveIsLegal<false, false, Color::White,Color::Black>(move, ci.m_pinned)
			)
		) {
			continue;
		}

		pFlashlight->m_currentMove = move;

		pos.GetTurn()==Color::Black
			?
			pos.DoMove<Color::Black,Color::White>(move, st, ci, givesCheck)
			:
			pos.DoMove<Color::White,Color::Black>(move, st, ci, givesCheck)
			;

		(pFlashlight + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
		score =
			// 静止探索☆　自分自身を呼び出している（再帰）ぜ☆（＾ｑ＾）
			-this->DoQsearch(rucksack, givesCheck, pos, pFlashlight + 1, -beta, -alpha, depth - OnePly);
		pos.UndoMove(move);

		assert(-ScoreInfinite < score && score < ScoreInfinite);

		if (bestScore < score) {
			bestScore = score;

			if (alpha < score) {
				// PVノードのときは条件付きで手続きが変わるぜ☆（＾ｑ＾）
				bool isReturnWithScore = false;
				ScoreIndex returnScore = ScoreIndex::ScoreNone;
				this->DoByNewScore(
					isReturnWithScore,
					returnScore,
					rucksack,
					score,
					beta,
					alpha,
					bestMove,
					posKey,
					&pFlashlight,
					ttDepth,
					move
					);
				if (isReturnWithScore) {
					return returnScore;
				}
			}
		}
	}

	if (INCHECK && bestScore == -ScoreInfinite) {
		return UtilScore::MatedIn(pFlashlight->m_ply);
	}

	rucksack.m_tt.Store(
		posKey,
		rucksack.ConvertScoreToTT(bestScore, pFlashlight->m_ply),
		this->GetBound01( oldAlpha, bestScore),
		ttDepth,
		bestMove,
		pFlashlight->m_staticEval
	);

	assert(-ScoreInfinite < bestScore && bestScore < ScoreInfinite);

	return bestScore;
}
