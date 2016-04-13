#include "../../header/n119_score___/n119_090_score.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_500_moveStack.hpp"
#include "../../header/n165_movStack/n165_600_utilMove.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n220_position/n220_680_moveList.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_060_stats.hpp"
#include "../../header/n223_move____/n223_500_searchStack.hpp"
#include "../../header/n440_movStack/n440_500_movePicker.hpp"

#include "../../header/n450_movPhase/n450_100_mainSearch.hpp"
#include "../../header/n450_movPhase/n450_110_phTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_120_phKillers.hpp"
#include "../../header/n450_movPhase/n450_130_phNonTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_140_phNonTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_150_phBadCaptures.hpp"
#include "../../header/n450_movPhase/n450_160_evasionSearch.hpp"
#include "../../header/n450_movPhase/n450_170_phEvasions.hpp"
#include "../../header/n450_movPhase/n450_180_qSearch.hpp"
#include "../../header/n450_movPhase/n450_190_phQCaptures0.hpp"
#include "../../header/n450_movPhase/n450_200_qEvasionSearch.hpp"
#include "../../header/n450_movPhase/n450_210_phQEvasions.hpp"
#include "../../header/n450_movPhase/n450_220_probCut.hpp"
#include "../../header/n450_movPhase/n450_230_phTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_240_qRecapture.hpp"
#include "../../header/n450_movPhase/n450_250_phQCaptures1.hpp"
#include "../../header/n450_movPhase/n450_260_phStop.hpp"
#include "../../header/n450_movPhase/n450_500_movePhaseArray.hpp"


#include "../../header/n640_searcher/n640_440_splitPoint.hpp"	// 持ち合いになっているが .cpp だからいいかだぜ☆（＾ｑ＾）



using History = Stats<false>;


MovePicker::MovePicker(
	const Position& pos,
	const Move ttm,
	const Depth depth,
	const History& history,
	SearchStack* searchStack,
	const Score beta
)
	: m_pos_(pos), m_history_(history), m_depth_(depth)
{
	assert(Depth0 < depth);

	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット
	m_currMove_ = m_lastMove_ = GetFirstMove();
	m_captureThreshold_ = 0;
	m_endBadCaptures_ = m_legalMoves_ + g_MaxLegalMoves - 1;
	m_ss_ = searchStack;

	if (pos.InCheck()) {
		m_phase_ = N06_EvasionSearch;
	}
	else {
		m_phase_ = N00_MainSearch;

		m_killerMoves_[0].m_move = searchStack->m_killers[0];
		m_killerMoves_[1].m_move = searchStack->m_killers[1];

		if (m_ss_ != nullptr && m_ss_->m_staticEval < beta - g_CapturePawnScore && depth < 3 * OnePly) {
			m_captureThreshold_ = -g_CapturePawnScore;
		}
		else if (m_ss_ != nullptr && beta < m_ss_->m_staticEval) {
			m_captureThreshold_ = beta - m_ss_->m_staticEval;
		}
	}

	m_ttMove_ = (!ttm.IsNone() && pos.MoveIsPseudoLegal(ttm) ? ttm : Move::GetMoveNone());
	m_lastMove_ += (!m_ttMove_.IsNone());
}

// 静止探索で呼ばれる。
MovePicker::MovePicker(const Position& pos, Move ttm, const Depth depth, const History& history, const Square sq)
	: m_pos_(pos), m_history_(history), m_currMove_(GetFirstMove()), m_lastMove_(GetFirstMove())
{
	assert(depth <= Depth0);
	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット

	if (pos.InCheck())
		m_phase_ = N10_QEvasionSearch;
	// todo: ここで Stockfish は qcheck がある。
	else if (DepthQRecaptures < depth)
		m_phase_ = N08_QSearch;
	else {
		m_phase_ = N14_QRecapture;
		m_recaptureSquare_ = sq;
		ttm = Move::GetMoveNone();
	}

	m_ttMove_ = (!ttm.IsNone() && pos.MoveIsPseudoLegal(ttm) ? ttm : Move::GetMoveNone());
	m_lastMove_ += !m_ttMove_.IsNone();
}

MovePicker::MovePicker(const Position& pos, const Move ttm, const History& history, const PieceType pt)
	: m_pos_(pos), m_history_(history), m_currMove_(GetFirstMove()), m_lastMove_(GetFirstMove())
{
	assert(!pos.InCheck());

	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット
	m_phase_ = N12_ProbCut;

	m_captureThreshold_ = pos.GetCapturePieceScore(pt);
	m_ttMove_ = ((!ttm.IsNone() && pos.MoveIsPseudoLegal(ttm)) ? ttm : Move::GetMoveNone());

	if (!m_ttMove_.IsNone() && (!m_ttMove_.IsCapture() || pos.GetSee(m_ttMove_) <= m_captureThreshold_)) {
		m_ttMove_ = Move::GetMoveNone();
	}

	m_lastMove_ += !m_ttMove_.IsNone();
}

template <> Move MovePicker::GetNextMove<false>() {
	do {
		// lastMove() に達したら次の phase に移る。
		while (GetCurrMove() == GetLastMove()) {
			GoNextPhase();
		}

		Move resultMove;
		bool isGotNext = g_movePhaseArray[GetPhase()]->GetNext2Move(resultMove, *this);

		if (isGotNext) {
			return resultMove;
		}

	} while (true);
}

template <>
Move MovePicker::GetNextMove<true>() {
	return this->m_ss_->m_splitPoint->m_pMovePicker->GetNextMove<false>();
}

const Score LVATable[N15_PieceTypeNum] = {
	Score(0), Score(1), Score(2), Score(3), Score(4), Score(7), Score(8), Score(6), Score(10000),
	Score(5), Score(5), Score(5), Score(5), Score(9), Score(10)
};
inline Score LVA(const PieceType pt) { return LVATable[pt]; }

void MovePicker::ScoreCaptures() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->m_move;
		curr->m_score = Position::GetPieceScore(GetPos().GetPiece(move.To())) - LVA(move.GetPieceTypeFrom());
	}
}

void MovePicker::ScoreEvasions() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->m_move;
		const Score seeScore = GetPos().GetSeeSign(move);
		if (seeScore < 0) {
			curr->m_score = seeScore - History::m_MaxScore;
		}
		else if (move.IsCaptureOrPromotion()) {
			curr->m_score = GetPos().GetCapturePieceScore(GetPos().GetPiece(move.To())) + History::m_MaxScore;
			if (move.IsPromotion()) {
				const PieceType pt = UtilPiece::ToPieceType(GetPos().GetPiece(move.From()));
				curr->m_score += GetPos().GetPromotePieceScore(pt);
			}
		}
		else {
			curr->m_score = GetHistory().GetValue(move.IsDrop(), UtilPiece::FromColorAndPieceType(GetPos().GetTurn(), move.GetPieceTypeFromOrDropped()), move.To());
		}
	}
}

void MovePicker::GoNextPhase() {
	m_currMove_ = GetFirstMove(); // legalMoves_[0] は番兵
	++m_phase_;

	//g_movePhaseArray[GetPhase()]->GoNext2Phase(*this);

	/*
	switch (GetPhase()) {
	case N01_PH_TacticalMoves0:		g_phTacticalMoves0.GoNext2Phase(*this); return;
	case N13_PH_TacticalMoves1:		g_phTacticalMoves0.GoNext2Phase(*this); return;
	case N02_PH_Killers:			g_phKillers.GoNext2Phase(*this); return;
	case N03_PH_NonTacticalMoves0:	g_phNonTacticalMoves0.GoNext2Phase(*this); return;
	case N04_PH_NonTacticalMoves1:	g_phNonTacticalMoves1.GoNext2Phase(*this); return;
	case N05_PH_BadCaptures:		g_phBadCaptures.GoNext2Phase(*this); return;
	case N07_PH_Evasions:			g_phEvasions.GoNext2Phase(*this); return;
	case N11_PH_QEvasions:			g_phQEvasions.GoNext2Phase(*this); return;
	case N09_PH_QCaptures0:			g_phQCaptures0.GoNext2Phase(*this); return;
	case N15_PH_QCaptures1:			g_phQCaptures0.GoNext2Phase(*this); return;
	case N06_EvasionSearch:			g_evasionSearch.GoNext2Phase(*this); return;
	case N08_QSearch:				g_qSearch.GoNext2Phase(*this); return;
	case N10_QEvasionSearch:		g_qEvasionSearch.GoNext2Phase(*this); return;
	case N14_QRecapture:			g_qRecapture.GoNext2Phase(*this); return;
	case N12_ProbCut:				g_probCut.GoNext2Phase(*this); return;
	case N16_PH_Stop:				g_phStop.GoNext2Phase(*this); return;

	default: UNREACHABLE;
	}
	*/

	switch (GetPhase()) {
	case N01_PH_TacticalMoves0: case N13_PH_TacticalMoves1:
		m_lastMove_ = generateMoves<CapturePlusPro>(GetCurrMove(), GetPos());
		ScoreCaptures();
		return;

	case N02_PH_Killers:
		m_currMove_ = m_killerMoves_;
		m_lastMove_ = GetCurrMove() + 2;
		return;

	case N03_PH_NonTacticalMoves0:
		m_lastMove_ = generateMoves<NonCaptureMinusPro>(GetCurrMove(), GetPos());
		ScoreNonCapturesMinusPro<false>();
		m_currMove_ = GetLastMove();
		m_lastNonCapture_ = m_lastMove_ = generateMoves<Drop>(GetCurrMove(), GetPos());
		ScoreNonCapturesMinusPro<true>();
		m_currMove_ = GetFirstMove();
		m_lastMove_ = std::partition(GetCurrMove(), GetLastNonCapture(), HasPositiveScore());
		// 要素数は10個くらいまでであることが多い。要素数が少ないので、insertionSort() を使用する。
		UtilMoveStack::InsertionSort<MoveStack*, true>(GetCurrMove(), GetLastMove());
		return;

	case N04_PH_NonTacticalMoves1:
		m_currMove_ = GetLastMove();
		m_lastMove_ = GetLastNonCapture();
		if (static_cast<Depth>(3 * OnePly) <= m_depth_) {
			std::sort(GetCurrMove(), GetLastMove(), std::greater<MoveStack>());
		}
		return;

	case N05_PH_BadCaptures:
		m_currMove_ = m_legalMoves_ + g_MaxLegalMoves - 1;
		m_lastMove_ = m_endBadCaptures_;
		return;

	case N07_PH_Evasions:
	case N11_PH_QEvasions:
		m_lastMove_ = generateMoves<Evasion>(GetCurrMove(), GetPos());
		if (GetCurrMove() + 1 < GetLastMove()) {
			ScoreEvasions();
		}
		return;

	case N09_PH_QCaptures0:
		m_lastMove_ = generateMoves<CapturePlusPro>(GetFirstMove(), GetPos());
		ScoreCaptures();
		return;

	case N15_PH_QCaptures1:
		m_lastMove_ = generateMoves<Recapture>(GetFirstMove(), GetPos(), m_recaptureSquare_);
		ScoreCaptures();
		return;

	case N06_EvasionSearch: case N08_QSearch: case N10_QEvasionSearch: case N14_QRecapture: case N12_ProbCut:
		// これが無いと、MainSearch の後に EvasionSearch が始まったりしてしまう。
		m_phase_ = N16_PH_Stop;

	case N16_PH_Stop:
		m_lastMove_ = GetCurrMove() + 1;
		return;

	default: UNREACHABLE;
	}

}
