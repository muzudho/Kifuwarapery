#pragma once

/*
#include <iostream>
#include <algorithm>
#include "../n119_score___/n119_200_pieceScore.hpp"
#include "../n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../n160_board___/n160_220_queenAttackBb.hpp"
#include "../n160_board___/n160_230_setMaskBb.hpp"

#include "../n220_position/n220_100_repetitionType.hpp"
#include "../n220_position/n220_640_utilAttack.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "../n220_position/n220_750_charToPieceUSI.hpp"

#include "../n223_move____/n223_040_nodeType.hpp"
#include "../n223_move____/n223_300_moveAndScoreIndex.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"

#include "../n350_pieceTyp/n350_500_ptPrograms.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../n520_evaluate/n520_700_evaluation09.hpp"
#include "../n560_timeMng_/n560_500_timeManager.hpp"
#include "../n600_book____/n600_500_book.hpp"

#include "../n640_searcher/n640_440_splitedNode.hpp" // Searcherと持ち合い
#include "../n640_searcher/n640_500_reductions.hpp"
*/
#include "../n640_searcher/n640_510_futilityMargins.hpp"
#include "../n640_searcher/n640_520_futilityMoveCounts.hpp"
/*
#include "../n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../n680_egOption/n680_300_engineOptionSetup.hpp"
#include "../n760_thread__/n760_400_herosPub.hpp"

#include "../n800_learn___/n800_100_stopwatch.hpp"
#include "../n883_nodeType/n883_070_nodetypeAbstract.hpp"

#include "../n885_searcher/n885_040_rucksack.hpp"//FIXME:
#include "../n885_searcher/n885_310_hitchhikerQsearchAbstract.hpp"//FIXME:
*/
#include "../n885_searcher/n885_340_hitchhikerQsearchPrograms.hpp"//FIXME:
/*
#include "../n885_searcher/n885_480_hitchhikerNyugyoku.hpp"
#include "../n885_searcher/n885_510_hitchhiker.hpp"
#include "../n885_searcher/n885_600_iterativeDeepeningLoop.hpp"//FIXME:

#include "../n886_repeType/n886_100_rtNot.hpp"
#include "../n886_repeType/n886_110_rtDraw.hpp"
#include "../n886_repeType/n886_120_rtWin.hpp"
#include "../n886_repeType/n886_130_rtLose.hpp"
#include "../n886_repeType/n886_140_rtSuperior.hpp"
#include "../n886_repeType/n886_150_rtInferior.hpp"
*/
#include "../n886_repeType/n886_500_rtArray.hpp"//FIXME:
/*
//class Rucksack;

#include "../n887_nodeType/n887_500_nodetypePrograms.hpp"//FIXME:


using namespace std;


extern const InFrontMaskBb g_inFrontMaskBb;
*/
class NodetypeAbstract; // 同じファイル内の後ろの方でクラス定義があるとき☆（＾ｑ＾）
extern NodetypeAbstract* g_NODETYPE_PROGRAMS[];

extern RepetitionTypeArray g_repetitionTypeArray;


class NodetypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	virtual inline void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) const = 0;


	ScoreIndex GoToTheAdventure(
		Rucksack& rucksack,
		NodeType NT,//スレッドが実行するプログラムを切り替えます。
		Position& pos,
		Flashlight* pFlashlight,//サーチスタック
		ScoreIndex alpha,
		ScoreIndex beta,
		const Depth depth,
		const bool cutNode
		) const;


	// 非PVノードはassertをするぜ☆（＾ｑ＾）
	virtual inline void AssertBeforeStep1(
		ScoreIndex alpha,
		ScoreIndex beta
		) const {
		// 非PVノードのみ実行☆！（＾ｑ＾）
		assert(alpha == beta - 1);
	}

	// スプリット・ポイントのみ実行☆（＾ｑ＾）
	virtual inline void DoStep1a(
		bool& isGotoSplitPointStart,
		int& moveCount,
		int& playedMoveCount,
		bool& inCheck,
		Position& pos,
		SplitedNode** ppSplitedNode,
		Flashlight** ppFlashlight,
		Move& bestMove,
		Move& threatMove,
		ScoreIndex& bestScore,
		Move& ttMove,
		Move& excludedMove,
		ScoreIndex& ttScore
	)const {

		// initialize node

		*ppSplitedNode = (*ppFlashlight)->m_splitedNode;
		bestMove = (*ppSplitedNode)->m_bestMove;
		threatMove = (*ppSplitedNode)->m_threatMove;
		bestScore = (*ppSplitedNode)->m_bestScore;
		//tte = nullptr;
		ttMove = excludedMove = g_MOVE_NONE;
		ttScore = ScoreNone;

		Evaluation09 evaluation;
		evaluation.evaluate(pos, *ppFlashlight);

		assert(-ScoreInfinite < (*ppSplitedNode)->m_bestScore && 0 < (*ppSplitedNode)->m_moveCount);

		isGotoSplitPointStart = true;
		return;
		//goto split_point_start;
	}

	virtual inline void DoStep1b(
		ScoreIndex& bestScore,
		Flashlight** ppFlashlight,
		Move& threatMove,
		Move& bestMove
		)const {

		bestScore = -ScoreInfinite;
		(*ppFlashlight)->m_currentMove = threatMove = bestMove = ((*ppFlashlight) + 1)->m_excludedMove = g_MOVE_NONE;
		(*ppFlashlight)->m_ply = ((*ppFlashlight) - 1)->m_ply + 1;
		((*ppFlashlight) + 1)->m_skipNullMove = false;
		((*ppFlashlight) + 1)->m_reduction = Depth0;
		((*ppFlashlight) + 2)->m_killers[0] = ((*ppFlashlight) + 2)->m_killers[1] = g_MOVE_NONE;
	}

	virtual inline void DoStep1c(
		Military** ppThisThread,
		const Flashlight* pFlashlight
		)const {
		// PVノードのみ、最大Plyの更新の可能性があるぜ☆（＾ｑ＾）
		if ((*ppThisThread)->m_maxPly < pFlashlight->m_ply) {
			(*ppThisThread)->m_maxPly = pFlashlight->m_ply;
		}
	}

	// ルートノード以外が実行するぜ☆（＾ｑ＾）
	virtual inline void DoStep2(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Position& pos,
		Rucksack& rucksack,
		Flashlight** ppFlashlight
		)const
	{
		// stop と最大探索深さのチェック
		g_repetitionTypeArray.m_repetitionTypeArray[pos.IsDraw(16)]->CheckStopAndMaxPly(
			isReturnWithScore, returnScore, &rucksack, (*ppFlashlight));
	}

	// ルートノード以外が実行するぜ☆（＾ｑ＾）
	virtual inline void DoStep3(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Flashlight** ppFlashlight,
		ScoreIndex& alpha,
		ScoreIndex& beta
	)const {
		// ルート以外のみで行われる手続きだぜ☆（＾ｑ＾）！
		alpha = std::max(UtilScore::MatedIn((*ppFlashlight)->m_ply), alpha);
		beta = std::min(UtilScore::MateIn((*ppFlashlight)->m_ply + 1), beta);
		if (beta <= alpha) {
			isReturnWithScore = true;
			returnScore = alpha;
			return;
			//return alpha;
		}
	}

	virtual inline void DoStep4(
		Move& excludedMove,
		Flashlight** ppFlashlight,
		Key& posKey,
		Position& pos,
		const TTEntry** ppTtEntry,//セットされるぜ☆（＾ｑ＾）
		Rucksack& rucksack,
		ScoreIndex& ttScore
	)const {
		// trans position table lookup
		excludedMove = (*ppFlashlight)->m_excludedMove;
		posKey = (excludedMove.IsNone() ? pos.GetKey() : pos.GetExclusionKey());
		(*ppTtEntry) = rucksack.m_tt.Probe(posKey);
		ttScore = ((*ppTtEntry) != nullptr ? rucksack.ConvertScoreFromTT((*ppTtEntry)->GetScore(), (*ppFlashlight)->m_ply) : ScoreNone);
	}

	// ルートノードか、それ以外かで　値が分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep4x(
		Move& ttMove,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		Position& pos
		)const = 0;

	// PVノードか、非PVノードかで実行条件が変わるぜ☆（＾ｑ＾）
	virtual inline void DoStep4y(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		const TTEntry* pTtEntry,
		const Depth depth,
		ScoreIndex& ttScore,
		ScoreIndex& beta,
		Flashlight** ppFlashlight,
		Move& ttMove
		)const {

		// ルートノード以外だけにある手続きだぜ☆（＾ｑ＾）
		if (pTtEntry != nullptr
			&& depth <= pTtEntry->GetDepth()
			&& ttScore != ScoreNone // アクセス競合が起きたときのみ、ここに引っかかる。
			&& this->GetConditionInStep4y(
				pTtEntry,
				beta,
				ttScore
				)
		){
			rucksack.m_tt.Refresh(pTtEntry);
			(*ppFlashlight)->m_currentMove = ttMove; // Move::moveNone() もありえる。

			if (beta <= ttScore
				&& !ttMove.IsNone()
				&& !ttMove.IsCaptureOrPawnPromotion()
				&& ttMove != (*ppFlashlight)->m_killers[0])
			{
				(*ppFlashlight)->m_killers[1] = (*ppFlashlight)->m_killers[0];
				(*ppFlashlight)->m_killers[0] = ttMove;
			}

			isReturnWithScore = true;
			returnScore = ttScore;
			return;
			//return ttScore;
		}
	}

	// PVノードか、非PVノードかで実行条件が変わるぜ☆（＾ｑ＾）
	virtual inline bool GetConditionInStep4y(
		const TTEntry* pTtEntry,
		ScoreIndex& beta,
		ScoreIndex& ttScore
		) const = 0;

	virtual inline void DoStep4z(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		bool& inCheck,
		Move& move,
		Position& pos,
		Flashlight** ppFlashlight,
		ScoreIndex& bestScore,
		Key& posKey,
		const Depth depth,
		Move& bestMove
		)const {
		// ルートノード以外だけにある手続きだぜ☆（＾ｑ＾）
#if 1
		if (!inCheck)
		{
			if (!(move = pos.GetMateMoveIn1Ply()).IsNone()) {
				(*ppFlashlight)->m_staticEval = bestScore = UtilScore::MateIn((*ppFlashlight)->m_ply);
				rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, (*ppFlashlight)->m_ply), BoundExact, depth,
					move, (*ppFlashlight)->m_staticEval);
				bestMove = move;

				isReturnWithScore = true;
				returnScore = bestScore;
				return;
				//return bestScore;
			}
		}
#endif
	}

	virtual inline void DoStep5(
		bool& isGotoIidStart,
		Rucksack& rucksack,
		ScoreIndex& eval,
		Flashlight** ppFlashlight,
		Position& pos,
		bool& inCheck,
		const TTEntry* pTtEntry,
		ScoreIndex& ttScore,
		Key& posKey,
		Move& move
	)const {
		// evaluate the position statically
		Evaluation09 evaluation;
		eval = (*ppFlashlight)->m_staticEval = evaluation.evaluate(pos, (*ppFlashlight)); // Bonanza の差分評価の為、evaluate() を常に呼ぶ。
		if (inCheck) {
			eval = (*ppFlashlight)->m_staticEval = ScoreNone;
			isGotoIidStart = true;
			return;
			//goto iid_start;
		}
		else if (pTtEntry != nullptr) {
			if (ttScore != ScoreNone
				&& (pTtEntry->GetType() & (eval < ttScore ? Bound::BoundLower : Bound::BoundUpper)))
			{
				eval = ttScore;
			}
		}
		else {
			rucksack.m_tt.Store(posKey, ScoreNone, BoundNone, DepthNone,
				g_MOVE_NONE, (*ppFlashlight)->m_staticEval);
		}

		// 一手前の指し手について、history を更新する。
		// todo: ここの条件はもう少し考えた方が良い。
		if ((move = ((*ppFlashlight) - 1)->m_currentMove) != g_MOVE_NULL
			&& ((*ppFlashlight) - 1)->m_staticEval != ScoreNone
			&& (*ppFlashlight)->m_staticEval != ScoreNone
			&& !move.IsCaptureOrPawnPromotion() // 前回(一手前)の指し手が駒取りでなかった。
			)
		{
			const Square to = move.To();
			rucksack.m_gains.Update(move.IsDrop(), pos.GetPiece(to), to, -((*ppFlashlight) - 1)->m_staticEval - (*ppFlashlight)->m_staticEval);
		}
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	// N02_NonPV扱いで実行する関数があるぜ、なんだこれ☆（＾ｑ＾）Qサーチは、スプリットポイントかどうかは見てないのかだぜ☆（＾ｑ＾）
	virtual inline void DoStep6_NonPV(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		const Depth depth,
		ScoreIndex& eval,
		ScoreIndex& beta,
		Move& ttMove,
		Position& pos,
		Flashlight** ppFlashlight
	)const {
		// razoring

		// 非PVノードだけが実行するぜ☆！（＾ｑ＾）
		if (depth < 4 * OnePly
			&& eval + rucksack.razorMargin(depth) < beta
			&& ttMove.IsNone()
			&& abs(beta) < ScoreMateInMaxPly)
		{
			const ScoreIndex rbeta = beta - rucksack.razorMargin(depth);
			const ScoreIndex s = HitchhikerQsearchPrograms::m_pHitchhikerQsearchPrograms[N02_NonPV]->DoQsearch(
				rucksack, false, pos, (*ppFlashlight), rbeta - 1, rbeta, Depth0);
			if (s < rbeta) {
				isReturnWithScore = true;
				returnScore = s;
				return;
				//return s;
			}
		}
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep7(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Flashlight** ppFlashlight,
		const Depth depth,
		ScoreIndex& beta,
		ScoreIndex& eval
	)const {
		// static null move pruning

		// 非PVノードだけが実行するぜ☆！（＾ｑ＾）
		if (!(*ppFlashlight)->m_skipNullMove
			&& depth < 4 * OnePly
			&& beta <= eval - g_futilityMargins.m_FutilityMargins[depth][0]
			&& abs(beta) < ScoreMateInMaxPly)
		{
			bool isReturnWithScore = true;
			returnScore = eval - g_futilityMargins.m_FutilityMargins[depth][0];
			//return eval - g_futilityMargins.m_FutilityMargins[depth][0];
			return;
		}
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	// N02_NonPV扱いで実行する関数があるぜ、なんだこれ☆（＾ｑ＾）
	virtual inline void DoStep8_NonPV(
		bool& isReturnWithScore,
		ScoreIndex& returnScore,
		Rucksack& rucksack,
		Flashlight** ppFlashlight,
		const Depth depth,
		ScoreIndex& beta,
		ScoreIndex& eval,
		Position& pos,
		StateInfo& st,
		ScoreIndex& alpha,
		const bool cutNode,
		Move& threatMove
	)const {

		// null move

		// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
		if (!(*ppFlashlight)->m_skipNullMove
			&& 2 * OnePly <= depth
			&& beta <= eval
			&& abs(beta) < ScoreMateInMaxPly)
		{
			(*ppFlashlight)->m_currentMove = g_MOVE_NULL;
			Depth reduction = static_cast<Depth>(3) * OnePly + depth / 4;

			if (beta < eval - PieceScore::m_pawn) {
				reduction += OnePly;
			}

			pos.DoNullMove(true, st);
			((*ppFlashlight) + 1)->m_staticEvalRaw = (*ppFlashlight)->m_staticEvalRaw; // 評価値の差分評価の為。
			((*ppFlashlight) + 1)->m_skipNullMove = true;

			ScoreIndex nullScore = (depth - reduction < OnePly ?
				//────────────────────────────────────────────────────────────────────────────────
				// 深さが２手（先後１組）以上なら　クイックな探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				-HitchhikerQsearchPrograms::m_pHitchhikerQsearchPrograms[N02_NonPV]->DoQsearch(
					rucksack, false, pos, (*ppFlashlight) + 1, -beta, -alpha, Depth0)
				//────────────────────────────────────────────────────────────────────────────────
				// 深さが２手（先後１組）未満なら　ふつーの探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				:
				-g_NODETYPE_PROGRAMS[NodeType::N02_NonPV]->GoToTheAdventure(rucksack, NodeType::N02_NonPV, pos, (*ppFlashlight) + 1, -beta, -alpha, depth - reduction, !cutNode)
			);

			((*ppFlashlight) + 1)->m_skipNullMove = false;
			pos.DoNullMove(false, st);

			if (beta <= nullScore) {
				if (ScoreMateInMaxPly <= nullScore) {
					nullScore = beta;
				}

				if (depth < 6 * OnePly) {
					isReturnWithScore = true;
					returnScore = nullScore;
					return;
					//return nullScore;
				}

				(*ppFlashlight)->m_skipNullMove = true;
				assert(Depth0 < depth - reduction);
				//────────────────────────────────────────────────────────────────────────────────
				// 探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				const ScoreIndex s = g_NODETYPE_PROGRAMS[NodeType::N02_NonPV]->GoToTheAdventure(rucksack, NodeType::N02_NonPV, pos, (*ppFlashlight), alpha, beta, depth - reduction, false);
				(*ppFlashlight)->m_skipNullMove = false;

				if (beta <= s) {
					isReturnWithScore = true;
					returnScore = nullScore;
					return;
					//return nullScore;
				}
			}
			else {
				// fail low
				threatMove = ((*ppFlashlight) + 1)->m_currentMove;
				if (depth < 5 * OnePly
					&& ((*ppFlashlight) - 1)->m_reduction != Depth0
					&& !threatMove.IsNone()
					&& rucksack.allows(pos, ((*ppFlashlight) - 1)->m_currentMove, threatMove))
				{
					isReturnWithScore = true;
					returnScore = beta - 1;
					return;
					//return beta - 1;
				}
			}
		}
	}

	// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep9(
		bool& isReturnWithScore,
		Rucksack& rucksack,
		const Depth& depth,
		Flashlight** ppFlashlight,
		ScoreIndex& beta,
		Move& move,
		Position& pos,
		Move& ttMove,
		StateInfo& st,
		ScoreIndex& score,
		const bool cutNode
		)const {

		// probcut

		// 非PVノードだけが実行する手続きだぜ☆！（＾ｑ＾）
		if (5 * OnePly <= depth
			&& !(*ppFlashlight)->m_skipNullMove
			// 確実にバグらせないようにする。
			&& abs(beta) < ScoreInfinite - 200)
		{
			const ScoreIndex rbeta = beta + 200;
			const Depth rdepth = depth - OnePly - 3 * OnePly;

			assert(OnePly <= rdepth);
			assert(!((*ppFlashlight) - 1)->m_currentMove.IsNone());
			assert(((*ppFlashlight) - 1)->m_currentMove != g_MOVE_NULL);

			assert(move == (ppFlashlight - 1)->m_currentMove);
			// move.cap() は前回(一手前)の指し手で取った駒の種類
			NextmoveEvent mp(pos, ttMove, rucksack.m_history, move.GetCap());
			const CheckInfo ci(pos);
			while (!(move = mp.GetNextMove_NonSplitedNode()).IsNone()) {
				if (pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
					(*ppFlashlight)->m_currentMove = move;
					pos.DoMove(move, st, ci, pos.IsMoveGivesCheck(move, ci));
					((*ppFlashlight) + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;

					//────────────────────────────────────────────────────────────────────────────────
					// 探索☆？（＾ｑ＾）
					//────────────────────────────────────────────────────────────────────────────────
					score =	-g_NODETYPE_PROGRAMS[NodeType::N02_NonPV]->GoToTheAdventure(rucksack, NodeType::N02_NonPV, pos, (*ppFlashlight) + 1, -rbeta, -rbeta + 1, rdepth, !cutNode);
					pos.UndoMove(move);
					if (rbeta <= score) {
						isReturnWithScore = true;
						return;
						//return score;
					}
				}
			}
		}
	}

	// PVノードか、そうでないかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void DoStep10(
		const Depth depth,
		Move& ttMove,
		bool& inCheck,
		ScoreIndex& beta,
		Flashlight** ppFlashlight,
		Rucksack& rucksack,
		Position& pos,
		ScoreIndex& alpha,
		const TTEntry** ppTtEntry,//セットされるぜ☆
		Key& posKey
		)const = 0;

	// これはベータ値☆ PVノードか、そうでないかで値が変わるぜ☆（＾ｑ＾）
	virtual inline ScoreIndex GetBetaAtStep11(
		ScoreIndex beta
		) const = 0;

	// これはムーブ☆
	virtual inline Move GetMoveAtStep11(
		NextmoveEvent& mp
		) const = 0;

	// ルートノードか、そうでないかで分かれるぜ☆（＾ｑ＾）
	virtual inline void DoStep11A_BeforeLoop_SplitPointStart(
		Move& ttMove,
		const Depth depth,
		ScoreIndex& score,
		ScoreIndex& bestScore,
		bool& singularExtensionNode,
		Move& excludedMove,
		const TTEntry* pTtEntry
		)const = 0;

	virtual inline void DoStep11Ba_LoopHeader(
		bool& isContinue,
		const Move& move,
		const Move& excludedMove
		)const
	{
		if (move == excludedMove) {
			isContinue = true;
			return;
		}
	}

	virtual inline void DoStep11Bb_LoopHeader(
		bool& isContinue,
		const Rucksack& rucksack,
		const Move& move
		)const {
		// ルートノードにのみある手続きだぜ☆！（＾ｑ＾）
		if (std::find(rucksack.m_rootMoves.begin() + rucksack.m_pvIdx,
				rucksack.m_rootMoves.end(),
				move) == rucksack.m_rootMoves.end())
		{
			isContinue = true;
			return;
		}
	}

	// スプリット・ポイントかどうかで変わる手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep11Bb_LoopHeader(
		bool& isContinue,
		Position& pos,
		Move& move,
		const CheckInfo& ci,
		int& moveCount,
		SplitedNode** ppSplitedNode
		) const = 0;

	// ルートノードだけ実行する手続きだぜ☆（＾ｑ＾）
	virtual inline void DoStep11Bc_LoopHeader(
		Rucksack& rucksack,
		int& moveCount
		) const {
		rucksack.m_signals.m_firstRootMove = (moveCount == 1);
#if 0
		if (GetThisThread == rucksack.m_ownerHerosPub.GetFirstCaptain() && 3000 < rucksack.m_stopwatch.GetElapsed()) {
			SYNCCOUT << "info depth " << GetDepth / OnePly
				<< " currmove " << GetMove.ToUSI()
				<< " currmovenumber " << rucksack.m_moveCount + rucksack.m_pvIdx << SYNCENDL;
		}
#endif
	}

	virtual inline void DoStep11B_LoopHeader(
		Depth& extension,
		bool& captureOrPawnPromotion,
		Move& move,
		bool& givesCheck,
		const CheckInfo& ci,
		Position& pos,
		bool& dangerous
		)const
	{
		extension = Depth0;
		captureOrPawnPromotion = move.IsCaptureOrPawnPromotion();
		givesCheck = pos.IsMoveGivesCheck(move, ci);
		dangerous = givesCheck; // todo: not implement
	}

	virtual inline void DoStep12(
		Rucksack& rucksack,
		bool& givesCheck,
		Position& pos,
		Move& move,
		Depth& extension,
		bool& singularExtensionNode,
		Move& ttMove,
		ScoreIndex& ttScore,
		const CheckInfo& ci,
		const Depth depth,
		Flashlight** ppFlashlight,
		ScoreIndex& score,
		const bool cutNode,
		ScoreIndex& beta,
		Depth& newDepth
		)const {

		if (givesCheck && ScoreIndex::ScoreZero <= pos.GetSeeSign(move))
		{
			extension = Depth::OnePly;
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
			(*ppFlashlight)->m_excludedMove = move;
			(*ppFlashlight)->m_skipNullMove = true;
			//────────────────────────────────────────────────────────────────────────────────
			// 探索☆？（＾ｑ＾）
			//────────────────────────────────────────────────────────────────────────────────
			score =	g_NODETYPE_PROGRAMS[N02_NonPV]->GoToTheAdventure(rucksack, N02_NonPV, pos, (*ppFlashlight), rBeta - 1, rBeta, depth / 2, cutNode);
			(*ppFlashlight)->m_skipNullMove = false;
			(*ppFlashlight)->m_excludedMove = g_MOVE_NONE;

			if (score < rBeta) {
				//extension = OnePly;
				extension = (beta <= rBeta ? OnePly + OnePly / 2 : OnePly);
			}
		}

		newDepth = depth - OnePly + extension;
	}

	// 非PVノードだけ実行するぜ☆！（＾ｑ＾）
	virtual inline void DoStep13a(
		bool& isContinue,
		Rucksack& rucksack,
		bool& captureOrPawnPromotion,
		bool& inCheck,
		bool& dangerous,
		ScoreIndex& bestScore,
		Move& move,
		Move& ttMove,
		const Depth depth,
		int& moveCount,
		Move& threatMove,
		Position& pos,
		SplitedNode** ppSplitedNode,
		Depth& newDepth,
		Flashlight** ppFlashlight,
		ScoreIndex& beta
		) const {

		// futility pruning
		if (!captureOrPawnPromotion
			&& !inCheck
			&& !dangerous
			//&& move != ttMove // 次の行がtrueならこれもtrueなので条件から省く。
			&& ScoreMatedInMaxPly < bestScore)
		{
			assert(move != ttMove);
			// move count based pruning
			if (depth < 16 * OnePly
				&& g_futilityMoveCounts.m_futilityMoveCounts[depth] <= moveCount
				&& (threatMove.IsNone() || !rucksack.refutes(pos, move, threatMove)))
			{
				this->LockInStep13a(ppSplitedNode);
				isContinue = true;
				return;
			}

			// score based pruning
			const Depth predictedDepth = this->GetPredictedDepthInStep13a( newDepth, depth, moveCount);
			// gain を 2倍にする。
			const ScoreIndex futilityScore = (*ppFlashlight)->m_staticEval + g_futilityMargins.GetFutilityMargin(predictedDepth, moveCount)
				+ 2 * rucksack.m_gains.GetValue(move.IsDrop(), ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(pos.GetTurn(), move.GetPieceTypeFromOrDropped()), move.To());

			if (futilityScore < beta) {
				bestScore = std::max(bestScore, futilityScore);
				this->LockAndUpdateBestScoreInStep13a(
					ppSplitedNode,
					bestScore
					);
				isContinue = true;
				return;
			}

			if (predictedDepth < 4 * OnePly
				&& pos.GetSeeSign(move) < ScoreZero)
			{
				this->LockInStep13a(ppSplitedNode);
				isContinue = true;
				return;
			}
		}
	}

	// PVノードか、そうでないかで変わるぜ☆！（＾ｑ＾）
	virtual inline const Depth GetPredictedDepthInStep13a(
		Depth& newDepth,
		const Depth depth,
		int& moveCount
		) const = 0;

	// スプリット・ポイントでだけ実行☆（＾ｑ＾）！
	virtual inline void LockInStep13a(
		SplitedNode** ppSplitedNode
		) const
	{
		(*ppSplitedNode)->m_mutex.lock();
	}
	// スプリット・ポイントでだけ実行☆（＾ｑ＾）！
	virtual inline void LockAndUpdateBestScoreInStep13a(
		SplitedNode** ppSplitedNode,
		ScoreIndex& bestScore
		) const {

		(*ppSplitedNode)->m_mutex.lock();
		if ((*ppSplitedNode)->m_bestScore < bestScore) {
			(*ppSplitedNode)->m_bestScore = bestScore;
		}
	}

	// ルートノード、スプリットポイントはしない手続きだぜ☆！（＾ｑ＾）
	virtual inline void DoStep13b(
		Position& pos,
		Move& move,
		const CheckInfo& ci,
		int& moveCount,
		bool& isContinue
		) const {
		// RootNode, SPNode はすでに合法手であることを確認済み。
		if (!pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
			--moveCount;
			isContinue = true;
			return;
		}
	}

	// スプリット・ポイントか、PVノードかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void DoStep13c(
		bool& isContinue,
		Rucksack& rucksack,
		bool& captureOrPawnPromotion,
		bool& inCheck,
		bool& dangerous,
		ScoreIndex& bestScore,
		Move& move,
		Move& ttMove,
		const Depth depth,
		int& moveCount,
		Move& threatMove,
		Position& pos,
		SplitedNode** ppSplitedNode,
		Depth& newDepth,
		Flashlight** ppFlashlight,
		ScoreIndex& beta,
		const CheckInfo& ci,
		bool& isPVMove,
		int& playedMoveCount,
		Move movesSearched[64]
		)const = 0;

	// 非スプリットポイントのみ実行☆（＾ｑ＾）
	virtual inline void DoStep13d(
		bool& captureOrPawnPromotion,
		int& playedMoveCount,
		Move movesSearched[64],
		Move& move
		) const {

		if (!captureOrPawnPromotion && playedMoveCount < 64) {
			movesSearched[playedMoveCount++] = move;
		}
	}

	virtual inline void DoStep14(
		Position& pos,
		Move& move,
		StateInfo& st,
		const CheckInfo& ci,
		bool& givesCheck,
		Flashlight** ppFlashlight
		)const {
		pos.DoMove(move, st, ci, givesCheck);
		((*ppFlashlight) + 1)->m_staticEvalRaw.m_p[0][0] = ScoreIndex::ScoreNotEvaluated;
	}

	virtual inline void DoStep15(
		Rucksack& rucksack,
		const Depth depth,
		bool& isPVMove,
		bool& captureOrPawnPromotion,
		Move& move,
		Move& ttMove,
		Flashlight** ppFlashlight,
		int& moveCount,
		const bool cutNode,
		Depth& newDepth,
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode,
		ScoreIndex& score,
		Position& pos,
		bool& doFullDepthSearch
		)const {

		// LMR
		if (3 * OnePly <= depth
			&& !isPVMove
			&& !captureOrPawnPromotion
			&& move != ttMove
			&& (*ppFlashlight)->m_killers[0] != move
			&& (*ppFlashlight)->m_killers[1] != move)
		{
			this->SetReductionInStep15(
				ppFlashlight,
				depth,
				moveCount,
				cutNode
				);
			const Depth d = std::max(newDepth - (*ppFlashlight)->m_reduction, Depth::OnePly);

			// スプリットポイントではアルファを更新☆
			this->UpdateAlphaInStep15(
				alpha,
				ppSplitedNode
				);

			//────────────────────────────────────────────────────────────────────────────────
			// 探索☆？（＾ｑ＾）
			//────────────────────────────────────────────────────────────────────────────────
			// PVS
			score = -g_NODETYPE_PROGRAMS[N02_NonPV]->GoToTheAdventure(rucksack, N02_NonPV, pos, (*ppFlashlight) + 1, -(alpha + 1), -alpha, d, true);

			doFullDepthSearch = (alpha < score && (*ppFlashlight)->m_reduction != Depth0);
			(*ppFlashlight)->m_reduction = Depth0;
		}
		else {
			doFullDepthSearch = !isPVMove;
		}
	}

	// Pvノードかどうかで手続きが変わるぜ☆！（＾ｑ＾）
	virtual inline void SetReductionInStep15(
		Flashlight** ppFlashlight,
		const Depth depth,
		int& moveCount,
		const bool cutNode
		) const = 0;

	// スプリットノードだけが実行するぜ☆！（＾ｑ＾）
	virtual inline void UpdateAlphaInStep15(
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode
		) const {

		alpha = (*ppSplitedNode)->m_alpha;
	}

	// スプリットノードだけが実行するぜ☆！（＾ｑ＾）
	virtual inline void DoStep16a(
		bool& doFullDepthSearch,
		ScoreIndex& alpha,
		SplitedNode** ppSplitedNode
		)const {

		// full depth search
		// PVS
		if (doFullDepthSearch) {
			alpha = (*ppSplitedNode)->m_alpha;
		}
	}

	// （＾ｑ＾）N02_NonPV扱いで実行するみたいなんだがなんだこれだぜ☆
	virtual inline void DoStep16b_NonPVAtukai(
		Rucksack& rucksack,
		bool& doFullDepthSearch,
		ScoreIndex& score,
		Depth& newDepth,
		bool& givesCheck,
		Position& pos,
		Flashlight** ppFlashlight,
		ScoreIndex& alpha,
		const bool cutNode
		)const {

		// full depth search
		// PVS
		if (doFullDepthSearch) {
			score = (newDepth < OnePly ?
				(givesCheck ? -HitchhikerQsearchPrograms::m_pHitchhikerQsearchPrograms[N02_NonPV]->DoQsearch(rucksack, true, pos, (*ppFlashlight) + 1, -(alpha + 1), -alpha, Depth0)
					: -HitchhikerQsearchPrograms::m_pHitchhikerQsearchPrograms[N02_NonPV]->DoQsearch(rucksack, false, pos, (*ppFlashlight) + 1, -(alpha + 1), -alpha, Depth0))
				//────────────────────────────────────────────────────────────────────────────────
				// 探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				: -g_NODETYPE_PROGRAMS[N02_NonPV]->GoToTheAdventure(rucksack, N02_NonPV, pos, (*ppFlashlight) + 1, -(alpha + 1), -alpha, newDepth, !cutNode));
		}
	}

	// PVノードだけが実行するぜ☆！（＾ｑ＾）
	// N01_PV扱いで実行するみたいだが……☆（＾ｑ＾）
	virtual inline void DoStep16c(
		Rucksack& rucksack,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		ScoreIndex& beta,
		Depth& newDepth,
		bool& givesCheck,
		Position& pos,
		Flashlight** ppFlashlight
		)const {

		// 通常の探索
		if (
			isPVMove ||
			(alpha < score && this->IsBetaLargeAtStep16c(score,beta))
		) {
			score = (newDepth < OnePly ?
				(givesCheck ? -HitchhikerQsearchPrograms::m_pHitchhikerQsearchPrograms[N01_PV]->DoQsearch(rucksack, true, pos, (*ppFlashlight) + 1, -beta, -alpha, Depth0)
					: -HitchhikerQsearchPrograms::m_pHitchhikerQsearchPrograms[N01_PV]->DoQsearch(rucksack, false, pos, (*ppFlashlight) + 1, -beta, -alpha, Depth0))
				//────────────────────────────────────────────────────────────────────────────────
				// 探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				: -g_NODETYPE_PROGRAMS[N01_PV]->GoToTheAdventure(rucksack, N01_PV, pos, (*ppFlashlight) + 1, -beta, -alpha, newDepth, false));
		}
	}
	virtual inline bool IsBetaLargeAtStep16c(
		ScoreIndex& score,
		ScoreIndex& beta
		) const = 0;
		
	virtual inline void DoStep17(
		Position& pos,
		Move& move
		)const {
		pos.UndoMove(move);
	}

	// スプリット・ポイントだけが実行するぜ☆！（＾ｑ＾）
	virtual inline void DoStep18a(
		SplitedNode** ppSplitedNode,
		ScoreIndex& bestScore,
		ScoreIndex& alpha
		)const {
			(*ppSplitedNode)->m_mutex.lock();
			bestScore = (*ppSplitedNode)->m_bestScore;
			alpha = (*ppSplitedNode)->m_alpha;
	}

	// ルートノードだけが実行するぜ☆！（＾ｑ＾）
	virtual inline void DoStep18b(
		Rucksack& rucksack,
		Move& move,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		Position& pos
		) const{

		RootMove& rm = *std::find(rucksack.m_rootMoves.begin(), rucksack.m_rootMoves.end(), move);
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
				++rucksack.m_bestMoveChanges;
			}
		}
		else {
			rm.m_score_ = -ScoreInfinite;
		}
	}

	// スプリット・ポイントかどうかで分かれるぜ☆！（＾ｑ＾）
	virtual inline void DoStep18c(
		bool& isBreak,
		Rucksack& rucksack,
		Move& move,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		Position& pos,
		ScoreIndex& bestScore,
		SplitedNode** ppSplitedNode,
		Move& bestMove,
		ScoreIndex& beta
		)const = 0;

	// 非スプリットポイントでだけ実行するぜ☆（＾ｑ＾）
	virtual inline void DoStep19(
		bool& isBreak,
		Rucksack& rucksack,
		const Depth depth,
		Military** ppThisThread,
		ScoreIndex& bestScore,
		ScoreIndex& beta,
		Position& pos,
		Flashlight** ppFlashlight,
		ScoreIndex& alpha,
		Move& bestMove,
		Move& threatMove,
		int& moveCount,
		NextmoveEvent& mp,
		NodeType& nodeType,//スレッドが実行するプログラムを変えます。
		const bool cutNode
		)const {

		if (rucksack.m_ownerHerosPub.GetMinSplitDepth() <= depth
			&& rucksack.m_ownerHerosPub.GetAvailableSlave(*ppThisThread)
			&& (*ppThisThread)->m_splitedNodesSize < g_MaxSplitedNodesPerThread)
		{
			assert(bestScore < beta);
			(*ppThisThread)->ForkNewFighter<Rucksack::FakeSplit>(
				pos, *ppFlashlight, alpha, beta, bestScore, bestMove,
				depth, threatMove, moveCount, mp, nodeType, cutNode
				);
			if (beta <= bestScore) {
				isBreak = true;
				return;
			}
		}

	}

	// スプリット・ポイントは　ステップ２０を実行する前に終了するぜ☆（＾ｑ＾）
	virtual inline bool GetReturnBeforeStep20() const = 0;

	virtual inline void DoStep20(
		int& moveCount,
		Move& excludedMove,
		Rucksack& rucksack,
		ScoreIndex& alpha,
		Flashlight** ppFlashlight,//サーチスタック
		ScoreIndex& bestScore,
		int& playedMoveCount,
		ScoreIndex& beta,
		Key& posKey,
		const Depth depth,
		Move& bestMove,
		bool& inCheck,
		Position& pos,
		Move movesSearched[64]
	)const {
		if (moveCount == 0) {
			bestScore = !excludedMove.IsNone() ? alpha : UtilScore::MatedIn((*ppFlashlight)->m_ply);
			return;
		}

		if (bestScore == -ScoreInfinite) {
			assert(playedMoveCount == 0);
			bestScore = alpha;
		}

		if (beta <= bestScore) {
			// failed high
			rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, (*ppFlashlight)->m_ply), BoundLower, depth,
				bestMove, (*ppFlashlight)->m_staticEval);

			if (!bestMove.IsCaptureOrPawnPromotion() && !inCheck) {
				if (bestMove != (*ppFlashlight)->m_killers[0]) {
					(*ppFlashlight)->m_killers[1] = (*ppFlashlight)->m_killers[0];
					(*ppFlashlight)->m_killers[0] = bestMove;
				}

				const ScoreIndex bonus = static_cast<ScoreIndex>(depth * depth);
				const Piece pc1 = ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(pos.GetTurn(), bestMove.GetPieceTypeFromOrDropped());
				rucksack.m_history.Update(bestMove.IsDrop(), pc1, bestMove.To(), bonus);

				for (int i = 0; i < playedMoveCount - 1; ++i) {
					const Move m = movesSearched[i];
					const Piece pc2 = ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(pos.GetTurn(), m.GetPieceTypeFromOrDropped());
					rucksack.m_history.Update(m.IsDrop(), pc2, m.To(), -bonus);
				}
			}
		}
		else {
			// failed low or PV search
			rucksack.m_tt.Store(
				posKey,
				rucksack.ConvertScoreToTT(bestScore, (*ppFlashlight)->m_ply),
				this->GetBoundAtStep20(!bestMove.IsNone()),
				depth,
				bestMove,
				(*ppFlashlight)->m_staticEval
			);
		}

		assert(-ScoreIndex::ScoreInfinite < bestScore && bestScore < ScoreIndex::ScoreInfinite);
	}

	// スタティック・メソッドは継承できないので、スタティックにはしないぜ☆（＾ｑ＾）
	virtual inline Bound GetBoundAtStep20(bool bestMoveExists) const = 0;


};

