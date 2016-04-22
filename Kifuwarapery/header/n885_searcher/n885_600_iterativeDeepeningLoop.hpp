#pragma once


#include "../n080_common__/n080_100_common.hpp"
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_665_utilMoveStack.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "n885_500_rucksack.hpp"
#include "n885_580_skill.hpp"


// 深い反復ループ☆？（iterative deepening loop）
// 反復深化探索のことなのかだぜ☆（＾ｑ＾）？
class IterativeDeepeningLoop {
public:

	static inline void Execute885_500(Rucksack& rucksack, Position& pos)
	{
		Flashlight flashlight[g_maxPlyPlus2];
		Ply depth;
		Ply prevBestMoveChanges;
		ScoreIndex bestScore = -ScoreInfinite;
		ScoreIndex delta = -ScoreInfinite;
		ScoreIndex alpha = -ScoreInfinite;
		ScoreIndex beta = ScoreInfinite;
		bool bestMoveNeverChanged = true;
		int lastInfoTime = -1; // 将棋所のコンソールが詰まる問題への対処用

		memset(flashlight, 0, 4 * sizeof(Flashlight));
		rucksack.m_bestMoveChanges = 0;
#if defined LEARN
		// 高速化の為に浅い探索は反復深化しないようにする。学習時は浅い探索をひたすら繰り返す為。
		GetDepth = std::max<Ply>(0, m_limits.GetDepth - 1);
#else
		depth = 0;
#endif

		flashlight[0].m_currentMove = g_MOVE_NULL; // skip update gains
		rucksack.m_tt.NewSearch();
		rucksack.m_history.Clear();
		rucksack.m_gains.Clear();

		// マルチＰＶの数☆？
		rucksack.m_pvSize = rucksack.m_engineOptions["MultiPV"];
		Skill skill(rucksack.m_engineOptions["Skill_Level"], rucksack.m_engineOptions["Max_Random_Score_Diff"]);

		if (rucksack.m_engineOptions["Max_Random_Score_Diff_Ply"] < pos.GetGamePly()) {
			skill.m_maxRandomScoreDiff = ScoreZero;
			rucksack.m_pvSize = 1;
			assert(!skill.enabled()); // level による設定が出来るようになるまでは、これで良い。
		}

		if (skill.enabled() && rucksack.m_pvSize < 3) {
			rucksack.m_pvSize = 3;
		}
		rucksack.m_pvSize = std::min(rucksack.m_pvSize, rucksack.m_rootMoves.size());

		// 指し手が無ければ負け
		if (rucksack.m_rootMoves.empty()) {
			rucksack.m_rootMoves.push_back(RootMove(g_MOVE_NONE));
			SYNCCOUT << "info depth 0 score "
				<< rucksack.scoreToUSI(-ScoreMate0Ply)
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
		while (++depth <= g_maxPly && !rucksack.m_signals.m_stop && (!rucksack.m_limits.m_depth || depth <= rucksack.m_limits.m_depth)) {
			// 前回の iteration の結果を全てコピー
			for (size_t i = 0; i < rucksack.m_rootMoves.size(); ++i) {
				rucksack.m_rootMoves[i].m_prevScore_ = rucksack.m_rootMoves[i].m_score_;
			}

			prevBestMoveChanges = rucksack.m_bestMoveChanges;
			rucksack.m_bestMoveChanges = 0;

			// Multi PV loop
			for (rucksack.m_pvIdx = 0; rucksack.m_pvIdx < rucksack.m_pvSize && !rucksack.m_signals.m_stop; ++rucksack.m_pvIdx) {
#if defined LEARN
				m_alpha = rucksack.m_alpha;
				m_beta = rucksack.m_beta;
#else
				// aspiration search
				// alpha, beta をある程度絞ることで、探索効率を上げる。
				if (
					// 深さ５以上で
					5 <= depth &&
					abs(rucksack.m_rootMoves[rucksack.m_pvIdx].m_prevScore_) < PieceScore::m_ScoreKnownWin
					) {
					delta = static_cast<ScoreIndex>(16);
					alpha = rucksack.m_rootMoves[rucksack.m_pvIdx].m_prevScore_ - delta;
					beta = rucksack.m_rootMoves[rucksack.m_pvIdx].m_prevScore_ + delta;
				}
				else {
					alpha = -ScoreInfinite;
					beta = ScoreInfinite;
				}
#endif

				// aspiration search の window 幅を、初めは小さい値にして探索し、
				// fail high/low になったなら、今度は window 幅を広げて、再探索を行う。
				while (true) {
					// 探索を行う。
					flashlight->m_staticEvalRaw.m_p[0][0] = (flashlight + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
					bestScore = rucksack.Search(
						NodeType::N00_Root,
						pos, flashlight + 1, alpha, beta, static_cast<Depth>(depth * OnePly), false);
					// 先頭が最善手になるようにソート
					UtilMoveStack::InsertionSort(rucksack.m_rootMoves.begin() + rucksack.m_pvIdx, rucksack.m_rootMoves.end());

					for (size_t i = 0; i <= rucksack.m_pvIdx; ++i) {
						flashlight->m_staticEvalRaw.m_p[0][0] = (flashlight + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
						rucksack.m_rootMoves[i].InsertPvInTT(pos);
					}

#if 0
					// 詰みを発見したら即指す。
					if (ScoreMateInMaxPly <= abs(rucksack.m_bestScore) && abs(rucksack.m_bestScore) < ScoreInfinite) {
						SYNCCOUT << PvInfoToUSI(GetPos, rucksack.m_ply, rucksack.m_alpha, rucksack.m_beta) << SYNCENDL;
						rucksack.m_signals.m_stop = true;
					}
#endif

#if defined LEARN
					break;
#endif

					if (rucksack.m_signals.m_stop) {
						break;
					}

					if (alpha < bestScore && bestScore < beta) {
						break;
					}


					if (
						// 思考時間が3秒経過し、
						3000 < rucksack.m_stopwatch.GetElapsed()
						// 将棋所のコンソールが詰まるのを防ぐ。
						&& (depth < 10 || lastInfoTime + 200 < rucksack.m_stopwatch.GetElapsed()))
					{
						lastInfoTime = rucksack.m_stopwatch.GetElapsed();
						SYNCCOUT << rucksack.PvInfoToUSI(pos, depth, alpha, beta) << SYNCENDL;
					}

					// fail high/low のとき、aspiration window を広げる。
					if (PieceScore::m_ScoreKnownWin <= abs(bestScore)) {
						// 勝ち(負け)だと判定したら、最大の幅で探索を試してみる。
						alpha = -ScoreInfinite;
						beta = ScoreInfinite;
					}
					else if (beta <= bestScore) {
						beta += delta;
						delta += delta / 2;
					}
					else {
						rucksack.m_signals.m_failedLowAtRoot = true;
						rucksack.m_signals.m_stopOnPonderHit = false;

						alpha -= delta;
						delta += delta / 2;
					}

					assert(-ScoreInfinite <= alpha && beta <= ScoreInfinite);
				}

				UtilMoveStack::InsertionSort(rucksack.m_rootMoves.begin(), rucksack.m_rootMoves.begin() + rucksack.m_pvIdx + 1);

				if (
					(
						rucksack.m_pvIdx + 1 == rucksack.m_pvSize ||
						// 思考時間が3秒を経過し
						3000 < rucksack.m_stopwatch.GetElapsed()
						)
					// 将棋所のコンソールが詰まるのを防ぐ。
					&& (depth < 10 || lastInfoTime + 200 < rucksack.m_stopwatch.GetElapsed()))
				{
					lastInfoTime = rucksack.m_stopwatch.GetElapsed();
					SYNCCOUT << rucksack.PvInfoToUSI(pos, depth, alpha, beta) << SYNCENDL;
				}
			}

			//if (skill.enabled() && skill.timeToPick(depth)) {
			//	skill.pickMove();
			//}

			if (rucksack.m_limits.IsUseTimeManagement() && !rucksack.m_signals.m_stopOnPonderHit) {
				bool stop = false;

				// 深さが 5 ～ 49 で、PVサイズが 1 のとき。
				if (4 < depth && depth < 50 && rucksack.m_pvSize == 1) {
					rucksack.m_timeManager.SetPvInstability(rucksack.m_bestMoveChanges, prevBestMoveChanges);
				}

				// 次のイテレーションを回す時間が無いなら、ストップ
				if (
					// 有効時間の62%が、思考経過時間（ミリ秒）に満たない場合。
					(rucksack.m_timeManager.GetAvailableTime() * 62) / 100 < rucksack.m_stopwatch.GetElapsed()
					) {
					stop = true;
				}

				if (2 < depth && rucksack.m_bestMoveChanges) {
					bestMoveNeverChanged = false;
				}

				// 最善手が、ある程度の深さまで同じであれば、
				// その手が突出して良い手なのだろう。
				if (
					12 <= depth
					&& !stop
					&& bestMoveNeverChanged
					&& rucksack.m_pvSize == 1
					// ここは確実にバグらせないようにする。
					&& -ScoreInfinite + 2 * PieceScore::m_capturePawn <= bestScore
					&& (
						rucksack.m_rootMoves.size() == 1
						||
						// または、利用可能時間の40%が、思考経過時間未満の場合。
						rucksack.m_timeManager.GetAvailableTime() * 40 / 100
						<
						rucksack.m_stopwatch.GetElapsed()
						)
					) {
					const ScoreIndex rBeta = bestScore - 2 * PieceScore::m_capturePawn;
					(flashlight + 1)->m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
					(flashlight + 1)->m_excludedMove = rucksack.m_rootMoves[0].m_pv_[0];
					(flashlight + 1)->m_skipNullMove = true;
					const ScoreIndex s = rucksack.Search(
						NodeType::N02_NonPV, pos, flashlight + 1, rBeta - 1, rBeta, (depth - 3) * OnePly, true);
					(flashlight + 1)->m_skipNullMove = false;
					(flashlight + 1)->m_excludedMove = g_MOVE_NONE;

					if (s < rBeta) {
						stop = true;
					}
				}

				if (stop) {
					if (rucksack.m_limits.m_ponder) {
						rucksack.m_signals.m_stopOnPonderHit = true;
					}
					else {
						rucksack.m_signals.m_stop = true;
					}
				}
			}
		}
		skill.swapIfEnabled(&rucksack);
		SYNCCOUT << rucksack.PvInfoToUSI(pos, depth - 1, alpha, beta) << SYNCENDL;
	}

};