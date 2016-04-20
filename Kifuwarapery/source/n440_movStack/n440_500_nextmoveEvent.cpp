#include "../../header/n119_score___/n119_090_score.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_500_moveStack.hpp"
#include "../../header/n165_movStack/n165_600_utilMove.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_060_stats.hpp"
#include "../../header/n223_move____/n223_500_searchStack.hpp"
#include "../../header/n407_moveGen_/n407_900_moveList.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"

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


// （＾ｑ＾）元の名前は ＭｏｖｅＰｉｃｋｅｒ☆
// 指し手を選ぶ関数の引数として使われるぜ☆（＾ｑ＾）
NextmoveEvent::NextmoveEvent(
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
NextmoveEvent::NextmoveEvent(const Position& pos, Move ttm, const Depth depth, const History& history, const Square sq)
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

NextmoveEvent::NextmoveEvent(const Position& pos, const Move ttm, const History& history, const PieceType pt)
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

template <> Move NextmoveEvent::GetNextMove<false>() {
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
Move NextmoveEvent::GetNextMove<true>() {
	return this->m_ss_->m_splitPoint->m_pNextmoveEvent->GetNextMove<false>();
}

const Score LVATable[N15_PieceTypeNum] = {
	Score(0), Score(1), Score(2), Score(3), Score(4), Score(7), Score(8), Score(6), Score(10000),
	Score(5), Score(5), Score(5), Score(5), Score(9), Score(10)
};
inline Score LVA(const PieceType pt) { return LVATable[pt]; }

void NextmoveEvent::ScoreCaptures() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->m_move;
		curr->m_score = Position::GetPieceScore(GetPos().GetPiece(move.To())) - LVA(move.GetPieceTypeFrom());
	}
}

void NextmoveEvent::ScoreEvasions() {
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

void NextmoveEvent::GoNextPhase() {
	m_currMove_ = GetFirstMove(); // legalMoves_[0] は番兵
	++m_phase_;

	g_movePhaseArray[GetPhase()]->GoNext2Phase(*this);
}
