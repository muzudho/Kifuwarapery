#pragma once


#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n640_searcher/n640_440_splitedNode.hpp" // Searcherと持ち合い
#include "../n640_searcher/n640_500_reductions.hpp"
#include "../n640_searcher/n640_510_futilityMargins.hpp"
#include "../n640_searcher/n640_520_futilityMoveCounts.hpp"

#include "../n885_searcher/n885_040_rucksack.hpp"//FIXME:
//class Rucksack;

using namespace std;


class NodetypeAbstract {
public:

	// テンプレートを使っている関数で使うには、static にするしかないぜ☆（＾ｑ＾）
	virtual void GoSearch(Rucksack& searcher, Position& pos, Flashlight* ss, SplitedNode& sp) const = 0;

	virtual const bool IsPvNode() const = 0;
	virtual const bool IsSplitedNode() const = 0;
	virtual const bool IsRootNode() const = 0;

	void DoStep9(
		bool& isReturnWithScore,
		Rucksack& rucksack,
		const Depth& depth,
		Flashlight* ss,
		ScoreIndex& beta,
		Move& move,
		Position& pos,
		Move& ttMove,
		StateInfo& st,
		ScoreIndex& score,
		const bool& cutNode
		) {

		// probcut
		if (!this->IsPvNode()
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

					//────────────────────────────────────────────────────────────────────────────────
					// 探索☆？（＾ｑ＾）
					//────────────────────────────────────────────────────────────────────────────────
					score = -Hitchhiker::Travel_885_510(rucksack, NodeType::N02_NonPV, pos, ss + 1, -rbeta, -rbeta + 1, rdepth, !cutNode);
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

	void DoStep10(
		const Depth& depth,
		Move& ttMove,
		bool& inCheck,
		ScoreIndex& beta,
		Flashlight* ss,
		Rucksack& rucksack,
		Position& pos,
		ScoreIndex& alpha,
		const TTEntry* tte,
		Key& posKey
		)
	{
		// internal iterative deepening
		if ((this->IsPvNode() ? 5 * OnePly : 8 * OnePly) <= depth
			&& ttMove.IsNone()
			&& (this->IsPvNode() || (!inCheck && beta <= ss->m_staticEval + static_cast<ScoreIndex>(256))))
		{
			//const Depth d = depth - 2 * OnePly - (PVNode ? Depth0 : depth / 4);
			const Depth d = (this->IsPvNode() ? depth - 2 * OnePly : depth / 2);

			ss->m_skipNullMove = true;
			if (this->IsPvNode())
			{
				//────────────────────────────────────────────────────────────────────────────────
				// 探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				Hitchhiker::Travel_885_510(rucksack, NodeType::N01_PV, pos, ss, alpha, beta, d, true);
			}
			else
			{
				//────────────────────────────────────────────────────────────────────────────────
				// 探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				Hitchhiker::Travel_885_510(rucksack, NodeType::N02_NonPV, pos, ss, alpha, beta, d, true);
			}
			ss->m_skipNullMove = false;

			tte = rucksack.m_tt.Probe(posKey);
			ttMove = (tte != nullptr ?
				UtilMoveStack::Move16toMove(tte->GetMove(), pos) :
				g_MOVE_NONE);
		}
	}

	void DoStep11A_BeforeLoop_SplitPointStart(
		Move& ttMove,
		const Depth& depth,
		ScoreIndex& score,
		ScoreIndex& bestScore,
		bool& singularExtensionNode,
		Move& excludedMove,
		const TTEntry* tte
		)
	{
		//NextmoveEvent mp(pos, ttMove, depth, rucksack.m_history, ss, this->IsPvNode() ? -ScoreInfinite : beta);
		//const CheckInfo ci(pos);
		score = bestScore;
		singularExtensionNode =
			!this->IsRootNode()
			&& !this-IsSplitedNode()
			&& 8 * Depth::OnePly <= depth
			&& !ttMove.IsNone()
			&& excludedMove.IsNone()
			&& (tte->GetType() & BoundLower)
			&& depth - 3 * Depth::OnePly <= tte->GetDepth();
	}

	void DoStep11B_LoopHeader(
		Rucksack& rucksack,
		bool& isContinue,
		Move& move,
		Move& excludedMove,
		Position& pos,
		const CheckInfo& ci,
		int& moveCount,
		SplitedNode* splitedNode,
		Depth& extension,
		bool& captureOrPawnPromotion,
		bool& givesCheck,
		bool& dangerous
		)
	{
		if (move == excludedMove) {
			isContinue = true;
			return;
		}

		if (this->IsRootNode()
			&& std::find(rucksack.m_rootMoves.begin() + rucksack.m_pvIdx,
				rucksack.m_rootMoves.end(),
				move) == rucksack.m_rootMoves.end())
		{
			isContinue = true;
			return;
		}

		if (this->IsSplitedNode()) {
			if (!pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
				isContinue = true;
				return;
			}
			moveCount = ++splitedNode->m_moveCount;
			splitedNode->m_mutex.unlock();
		}
		else {
			++moveCount;
		}


		if (this->IsRootNode()) {
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

	}

	void DoStep12(
		Rucksack& rucksack,
		bool& givesCheck,
		Position& pos,
		Move& move,
		Depth& extension,
		bool& singularExtensionNode,
		Move& ttMove,
		ScoreIndex& ttScore,
		const CheckInfo& ci,
		const Depth& depth,
		Flashlight* ss,
		ScoreIndex& score,
		const bool& cutNode,
		ScoreIndex& beta,
		Depth& newDepth
		) {

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
			ss->m_excludedMove = move;
			ss->m_skipNullMove = true;
			//────────────────────────────────────────────────────────────────────────────────
			// 探索☆？（＾ｑ＾）
			//────────────────────────────────────────────────────────────────────────────────
			score = Hitchhiker::Travel_885_510(rucksack, N02_NonPV, pos, ss, rBeta - 1, rBeta, depth / 2, cutNode);
			ss->m_skipNullMove = false;
			ss->m_excludedMove = g_MOVE_NONE;

			if (score < rBeta) {
				//extension = OnePly;
				extension = (beta <= rBeta ? OnePly + OnePly / 2 : OnePly);
			}
		}

		newDepth = depth - OnePly + extension;
	}

	void DoStep13(
		bool& isContinue,
		Rucksack& rucksack,
		bool& captureOrPawnPromotion,
		bool& inCheck,
		bool& dangerous,
		ScoreIndex& bestScore,
		Move& move,
		Move& ttMove,
		const Depth& depth,
		int& moveCount,
		Move& threatMove,
		Position& pos,
		SplitedNode* splitedNode,
		Depth& newDepth,
		Flashlight& ss,
		ScoreIndex& beta,
		const CheckInfo& ci,
		bool& isPVMove,
		int& playedMoveCount,
		Move movesSearched[64]
		) {

		// futility pruning
		if (!this->IsPvNode()
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
				&& (threatMove.IsNone() || !rucksack.refutes(pos, move, threatMove)))
			{
				if (this->IsSplitedNode()) {
					splitedNode->m_mutex.lock();
				}
				isContinue = true;
				return;
			}

			// score based pruning
			const Depth predictedDepth = newDepth - g_reductions.reduction(this->IsPvNode(), depth, moveCount);
			// gain を 2倍にする。
			const ScoreIndex futilityScore = ss.m_staticEval + g_futilityMargins.GetFutilityMargin(predictedDepth, moveCount)
				+ 2 * rucksack.m_gains.GetValue(move.IsDrop(), ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(pos.GetTurn(), move.GetPieceTypeFromOrDropped()), move.To());

			if (futilityScore < beta) {
				bestScore = std::max(bestScore, futilityScore);
				if (this->IsSplitedNode()) {
					splitedNode->m_mutex.lock();
					if (splitedNode->m_bestScore < bestScore) {
						splitedNode->m_bestScore = bestScore;
					}
				}
				isContinue = true;
				return;
			}

			if (predictedDepth < 4 * OnePly
				&& pos.GetSeeSign(move) < ScoreZero)
			{
				if (this->IsSplitedNode()) {
					splitedNode->m_mutex.lock();
				}
				isContinue = true;
				return;
			}
		}

		// RootNode, SPNode はすでに合法手であることを確認済み。
		if (!this->IsRootNode() && !this->IsSplitedNode() && !pos.IsPseudoLegalMoveIsLegal<false, false>(move, ci.m_pinned)) {
			--moveCount;
			isContinue = true;
			return;
		}

		isPVMove = (this->IsPvNode() && moveCount == 1);
		ss.m_currentMove = move;
		if (!this->IsSplitedNode() && !captureOrPawnPromotion && playedMoveCount < 64) {
			movesSearched[playedMoveCount++] = move;
		}

	}

	void DoStep14(
		Position& pos,
		Move& move,
		StateInfo& st,
		const CheckInfo& ci,
		bool& givesCheck,
		Flashlight* ss
		) {
		pos.DoMove(move, st, ci, givesCheck);
		(ss + 1)->m_staticEvalRaw.m_p[0][0] = ScoreIndex::ScoreNotEvaluated;
	}

	void DoStep15(
		Rucksack& rucksack,
		const Depth& depth,
		bool& isPVMove,
		bool& captureOrPawnPromotion,
		Move& move,
		Move& ttMove,
		Flashlight* ss,
		const bool& PVNode,
		int& moveCount,
		const bool& cutNode,
		Depth& newDepth,
		ScoreIndex& alpha,
		SplitedNode* splitedNode,
		ScoreIndex& score,
		Position& pos,
		bool& doFullDepthSearch
		) {
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
			if (this->IsSplitedNode()) {
				alpha = splitedNode->m_alpha;
			}
			//────────────────────────────────────────────────────────────────────────────────
			// 探索☆？（＾ｑ＾）
			//────────────────────────────────────────────────────────────────────────────────
			// PVS
			score = -Hitchhiker::Travel_885_510(
				rucksack, N02_NonPV, pos, ss + 1, -(alpha + 1), -alpha, d, true);

			doFullDepthSearch = (alpha < score && ss->m_reduction != Depth0);
			ss->m_reduction = Depth0;
		}
		else {
			doFullDepthSearch = !isPVMove;
		}
	}

	void DoStep16(
		Rucksack& rucksack,
		bool& doFullDepthSearch,
		SplitedNode* splitedNode,
		ScoreIndex& alpha,
		ScoreIndex& score,
		Depth& newDepth,
		bool& givesCheck,
		Position& pos,
		Flashlight* ss,
		const bool& cutNode,
		bool& isPVMove,
		ScoreIndex& beta
		) {

		// full depth search
		// PVS
		if (doFullDepthSearch) {
			if (this->IsSplitedNode()) {
				alpha = splitedNode->m_alpha;
			}
			score = (newDepth < OnePly ?
				(givesCheck ? -Hitchhiker::Qsearch(rucksack, N02_NonPV, true, pos, ss + 1, -(alpha + 1), -alpha, Depth0)
					: -Hitchhiker::Qsearch(rucksack, N02_NonPV, false, pos, ss + 1, -(alpha + 1), -alpha, Depth0))
				//────────────────────────────────────────────────────────────────────────────────
				// 探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				: -Hitchhiker::Travel_885_510(
					rucksack, N02_NonPV, pos, ss + 1, -(alpha + 1), -alpha, newDepth, !cutNode));
		}

		// 通常の探索
		if (this->IsPvNode() && (isPVMove || (alpha < score && (this->IsRootNode() || score < beta)))) {
			score = (newDepth < OnePly ?
				(givesCheck ? -Hitchhiker::Qsearch(rucksack, N01_PV, true, pos, ss + 1, -beta, -alpha, Depth0)
					: -Hitchhiker::Qsearch(rucksack, N01_PV, false, pos, ss + 1, -beta, -alpha, Depth0))
				//────────────────────────────────────────────────────────────────────────────────
				// 探索☆？（＾ｑ＾）
				//────────────────────────────────────────────────────────────────────────────────
				: -Hitchhiker::Travel_885_510(
					rucksack, N01_PV, pos, ss + 1, -beta, -alpha, newDepth, false));
		}

	}

	void DoStep17(
		Position& pos,
		Move& move
		) {
		pos.UndoMove(move);
	}

	void DoStep18(
		bool& isBreak,
		Rucksack& rucksack,
		Move& move,
		bool& isPVMove,
		ScoreIndex& alpha,
		ScoreIndex& score,
		Position& pos,
		ScoreIndex& bestScore,
		SplitedNode* splitedNode,
		Move& bestMove,
		ScoreIndex& beta
		) {

		if (this->IsRootNode()) {
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

		if (bestScore < score) {
			bestScore = (this->IsSplitedNode() ? splitedNode->m_bestScore = score : score);

			if (alpha < score) {
				bestMove = (this->IsSplitedNode() ? splitedNode->m_bestMove = move : move);

				if (this->IsPvNode() && score < beta) {
					alpha = (this->IsSplitedNode() ? splitedNode->m_alpha = score : score);
				}
				else {
					// fail high
					if (this->IsSplitedNode()) {
						splitedNode->m_cutoff = true;
					}
					isBreak = true;
					return;
				}
			}
		}

	}

	void DoStep19(
		bool& isBreak,
		Rucksack& rucksack,
		const Depth& depth,
		Military* pThisThread,
		ScoreIndex& bestScore,
		ScoreIndex& beta,
		Position& pos,
		Flashlight& ss,
		ScoreIndex& alpha,
		Move& bestMove,
		Move& threatMove,
		int& moveCount,
		NextmoveEvent& mp,
		NodeType NT,
		const bool cutNode
		){
		if (!this->IsSplitedNode()
			&& rucksack.m_ownerHerosPub.GetMinSplitDepth() <= depth
			&& rucksack.m_ownerHerosPub.GetAvailableSlave(pThisThread)
			&& pThisThread->m_splitedNodesSize < g_MaxSplitedNodesPerThread)
		{
			assert(bestScore < beta);
			pThisThread->ForkNewFighter<Rucksack::FakeSplit>(
				pos, &ss, alpha, beta, bestScore, bestMove,
				depth, threatMove, moveCount, mp, NT, cutNode
				);
			if (beta <= bestScore) {
				isBreak = true;
				return;
			}
		}

	}

	void DoStep20(
		Rucksack& rucksack,
		ScoreIndex& alpha,
		Flashlight& ss,
		ScoreIndex& bestScore,
		int& playedMoveCount,
		ScoreIndex& beta,
		Key& posKey,
		const Depth depth,
		Move& bestMove,
		bool& inCheck,
		Position& pos,
		Move movesSearched[64]
	) {
		if (bestScore == -ScoreInfinite) {
			assert(playedMoveCount == 0);
			bestScore = alpha;
		}

		if (beta <= bestScore) {
			// failed high
			rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, ss.m_ply), BoundLower, depth,
				bestMove, ss.m_staticEval);

			if (!bestMove.IsCaptureOrPawnPromotion() && !inCheck) {
				if (bestMove != ss.m_killers[0]) {
					ss.m_killers[1] = ss.m_killers[0];
					ss.m_killers[0] = bestMove;
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
			rucksack.m_tt.Store(posKey, rucksack.ConvertScoreToTT(bestScore, ss.m_ply),
				((this->IsPvNode() && !bestMove.IsNone()) ? BoundExact : BoundUpper),
				depth, bestMove, ss.m_staticEval);
		}

	}
};


