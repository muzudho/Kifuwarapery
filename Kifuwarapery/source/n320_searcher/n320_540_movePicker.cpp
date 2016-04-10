#include "../../header/n119_score___/n119_090_score.hpp"
#include "../../header/n220_position/n220_700_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"
#include "../../header/n276_genMove_/n276_150_moveList.hpp"
#include "../../header/n276_genMove_/n276_140_makePromoteMove.hpp"
#include "../../header/n280_move____/n280_300_stats.hpp"
#include "../../header/n320_searcher/n320_125_searchStack.hpp"
#include "../../header/n320_searcher/n320_540_movePicker.hpp"
#include "../../header/n450_thread__/n450_200_splitPoint.hpp"
//#include "../../header/n450_thread__/n450_400_threadPool.hpp" // これを外すのは難しい。


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

	m_legalMoves_[0].score = INT_MAX; // 番兵のセット
	m_currMove_ = m_lastMove_ = GetFirstMove();
	m_captureThreshold_ = 0;
	m_endBadCaptures_ = m_legalMoves_ + g_MaxLegalMoves - 1;
	m_ss_ = searchStack;

	if (pos.InCheck()) {
		m_phase_ = EvasionSearch;
	}
	else {
		m_phase_ = MainSearch;

		m_killerMoves_[0].move = searchStack->m_killers[0];
		m_killerMoves_[1].move = searchStack->m_killers[1];

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
	m_legalMoves_[0].score = INT_MAX; // 番兵のセット

	if (pos.InCheck())
		m_phase_ = QEvasionSearch;
	// todo: ここで Stockfish は qcheck がある。
	else if (DepthQRecaptures < depth)
		m_phase_ = QSearch;
	else {
		m_phase_ = QRecapture;
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

	m_legalMoves_[0].score = INT_MAX; // 番兵のセット
	m_phase_ = ProbCut;

	m_captureThreshold_ = pos.GetCapturePieceScore(pt);
	m_ttMove_ = ((!ttm.IsNone() && pos.MoveIsPseudoLegal(ttm)) ? ttm : Move::GetMoveNone());

	if (!m_ttMove_.IsNone() && (!m_ttMove_.IsCapture() || pos.GetSee(m_ttMove_) <= m_captureThreshold_)) {
		m_ttMove_ = Move::GetMoveNone();
	}

	m_lastMove_ += !m_ttMove_.IsNone();
}

template <> Move MovePicker::GetNextMove<false>() {
	MoveStack* ms;
	Move move;
	do {
		// lastMove() に達したら次の phase に移る。
		while (GetCurrMove() == GetLastMove()) {
			GoNextPhase();
		}

		switch (GetPhase()) {

		case MainSearch: case EvasionSearch: case QSearch: case QEvasionSearch: case ProbCut:
			++m_currMove_;
			return m_ttMove_;

		case PH_TacticalMoves0:
			ms = UtilMoveStack::PickBest(m_currMove_++, GetLastMove());
			if (ms->move != m_ttMove_) {
				assert(m_captureThreshold_ <= 0);

				if (m_captureThreshold_ <= GetPos().GetSee(ms->move)) {
					return ms->move;
				}

				// 後ろから SEE の点数が高い順に並ぶようにする。
				(m_endBadCaptures_--)->move = ms->move;
			}
			break;

		case PH_Killers:
			move = (m_currMove_++)->move;
			if (!move.IsNone()
				&& move != m_ttMove_
				&& GetPos().MoveIsPseudoLegal(move, true)
				&& GetPos().GetPiece(move.To()) == Empty)
			{
				return move;
			}
			break;

		case PH_NonTacticalMoves0:
		case PH_NonTacticalMoves1:
			move = (m_currMove_++)->move;
			if (move != m_ttMove_
				&& move != m_killerMoves_[0].move
				&& move != m_killerMoves_[1].move
				)
			{
				return move;
			}
			break;

		case PH_BadCaptures:
			return (m_currMove_--)->move;

		case PH_Evasions: case PH_QEvasions: case PH_QCaptures0:
			move = UtilMoveStack::PickBest(m_currMove_++, GetLastMove())->move;
			if (move != m_ttMove_) {
				return move;
			}
			break;

		case PH_TacticalMoves1:
			ms = UtilMoveStack::PickBest(m_currMove_++, GetLastMove());
			// todo: see が確実に駒打ちじゃないから、内部で駒打ちか判定してるのは少し無駄。
			if (ms->move != m_ttMove_ && m_captureThreshold_ < GetPos().GetSee(ms->move)) {
				return ms->move;
			}
			break;

		case PH_QCaptures1:
			move = UtilMoveStack::PickBest(m_currMove_++, GetLastMove())->move;
			assert(move.To() == m_recaptureSquare_);
			return move;

		case PH_Stop:
			return Move::GetMoveNone();
		default:
			UNREACHABLE;
		}
	} while (true);
}

template <>
Move MovePicker::GetNextMove<true>() {
	return this->m_ss_->m_splitPoint->m_pMovePicker->GetNextMove<false>();
}

const Score LVATable[PieceTypeNum] = {
	Score(0), Score(1), Score(2), Score(3), Score(4), Score(7), Score(8), Score(6), Score(10000),
	Score(5), Score(5), Score(5), Score(5), Score(9), Score(10)
};
inline Score LVA(const PieceType pt) { return LVATable[pt]; }

void MovePicker::ScoreCaptures() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->move;
		curr->score = Position::GetPieceScore(GetPos().GetPiece(move.To())) - LVA(move.GetPieceTypeFrom());
	}
}

template <bool IsDrop> void MovePicker::ScoreNonCapturesMinusPro() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->move;
		curr->score =
			GetHistory().GetValue(IsDrop,
				UtilPiece::FromColorAndPieceType(GetPos().GetTurn(),
													 (IsDrop ? move.GetPieceTypeDropped() : move.GetPieceTypeFrom())),
							move.To());
	}
}

void MovePicker::ScoreEvasions() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->move;
		const Score seeScore = GetPos().GetSeeSign(move);
		if (seeScore < 0) {
			curr->score = seeScore - History::m_MaxScore;
		}
		else if (move.IsCaptureOrPromotion()) {
			curr->score = GetPos().GetCapturePieceScore(GetPos().GetPiece(move.To())) + History::m_MaxScore;
			if (move.IsPromotion()) {
				const PieceType pt = UtilPiece::ToPieceType(GetPos().GetPiece(move.From()));
				curr->score += GetPos().GetPromotePieceScore(pt);
			}
		}
		else {
			curr->score = GetHistory().GetValue(move.IsDrop(), UtilPiece::FromColorAndPieceType(GetPos().GetTurn(), move.GetPieceTypeFromOrDropped()), move.To());
		}
	}
}

struct HasPositiveScore { bool operator () (const MoveStack& ms) { return 0 < ms.score; } };

void MovePicker::GoNextPhase() {
	m_currMove_ = GetFirstMove(); // legalMoves_[0] は番兵
	++m_phase_;

	switch (GetPhase()) {
	case PH_TacticalMoves0: case PH_TacticalMoves1:
		m_lastMove_ = generateMoves<CapturePlusPro>(GetCurrMove(), GetPos());
		ScoreCaptures();
		return;

	case PH_Killers:
		m_currMove_ = m_killerMoves_;
		m_lastMove_ = GetCurrMove() + 2;
		return;

	case PH_NonTacticalMoves0:
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

	case PH_NonTacticalMoves1:
		m_currMove_ = GetLastMove();
		m_lastMove_ = GetLastNonCapture();
		if (static_cast<Depth>(3 * OnePly) <= m_depth_) {
			std::sort(GetCurrMove(), GetLastMove(), std::greater<MoveStack>());
		}
		return;

	case PH_BadCaptures:
		m_currMove_ = m_legalMoves_ + g_MaxLegalMoves - 1;
		m_lastMove_ = m_endBadCaptures_;
		return;

	case PH_Evasions:
	case PH_QEvasions:
		m_lastMove_ = generateMoves<Evasion>(GetCurrMove(), GetPos());
		if (GetCurrMove() + 1 < GetLastMove()) {
			ScoreEvasions();
		}
		return;

	case PH_QCaptures0:
		m_lastMove_ = generateMoves<CapturePlusPro>(GetFirstMove(), GetPos());
		ScoreCaptures();
		return;

	case PH_QCaptures1:
		m_lastMove_ = generateMoves<Recapture>(GetFirstMove(), GetPos(), m_recaptureSquare_);
		ScoreCaptures();
		return;

	case EvasionSearch: case QSearch: case QEvasionSearch: case QRecapture: case ProbCut:
		// これが無いと、MainSearch の後に EvasionSearch が始まったりしてしまう。
		m_phase_ = PH_Stop;

	case PH_Stop:
		m_lastMove_ = GetCurrMove() + 1;
		return;

	default: UNREACHABLE;
	}
}
