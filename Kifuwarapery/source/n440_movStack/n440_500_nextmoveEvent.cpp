#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_500_moveStack.hpp"
#include "../../header/n165_movStack/n165_600_convMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_060_stats.hpp"
#include "../../header/n223_move____/n223_500_flashlight.hpp"

#include "../../header/n350_pieceTyp/n350_030_makePromoteMove.hpp"
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


#include "../../header/n640_searcher/n640_440_splitedNode.hpp"	// 持ち合いになっているが .cpp だからいいかだぜ☆（＾ｑ＾）



using History = Stats<false>;


// （＾ｑ＾）元の名前は ＭｏｖｅＰｉｃｋｅｒ☆
// 指し手を選ぶ関数の引数として使われるぜ☆（＾ｑ＾）
NextmoveEvent::NextmoveEvent(
	const Position& pos,
	const Move ttm,
	const Depth depth,
	const History& history,
	Flashlight* pFlashlightBox,
	const ScoreIndex beta
)
	: m_pos_(pos), m_history_(history), m_depth_(depth)
{
	assert(Depth0 < depth);

	this->m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット
	this->m_currMove_ = this->m_lastMove_ = GetFirstMove();
	this->m_captureThreshold_ = 0;
	this->m_endBadCaptures_ = this->m_legalMoves_ + Move::m_MAX_LEGAL_MOVES - 1;
	this->m_pFlashlightBox_ = pFlashlightBox;

	if (pos.InCheck()) {
		this->m_phase_ = N06_EvasionSearch;
	}
	else {
		this->m_phase_ = N00_MainSearch;

		this->m_killerMoves_[0].m_move = pFlashlightBox->m_killers[0];
		this->m_killerMoves_[1].m_move = pFlashlightBox->m_killers[1];

		if (
			this->m_pFlashlightBox_ != nullptr &&
			this->m_pFlashlightBox_->m_staticEval < beta - PieceScore::m_capturePawn &&
			depth < 3 * OnePly
		) {
			this->m_captureThreshold_ = -PieceScore::m_capturePawn;
		}
		else if (m_pFlashlightBox_ != nullptr && beta < this->m_pFlashlightBox_->m_staticEval) {
			this->m_captureThreshold_ = beta - this->m_pFlashlightBox_->m_staticEval;
		}
	}

	this->m_ttMove_ = (!ttm.IsNone() &&
		(
			pos.GetTurn() == Color::Black
			?
			pos.MoveIsPseudoLegal<Color::Black,Color::White>(ttm)
			:
			pos.MoveIsPseudoLegal<Color::White,Color::Black>(ttm)
		)		
		? ttm : g_MOVE_NONE);
	this->m_lastMove_ += (!this->m_ttMove_.IsNone());
}

// 静止探索で呼ばれる。
NextmoveEvent::NextmoveEvent(
	const Position& pos, Move ttm, const Depth depth, const History& history, const Square sq
)
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
		ttm = g_MOVE_NONE;
	}

	m_ttMove_ = (!ttm.IsNone() &&
		(
			pos.GetTurn()==Color::Black
			?
			pos.MoveIsPseudoLegal<Color::Black,Color::White>(ttm)
			:
			pos.MoveIsPseudoLegal<Color::White,Color::Black>(ttm)
		)		
		? ttm : g_MOVE_NONE);
	m_lastMove_ += !m_ttMove_.IsNone();
}

NextmoveEvent::NextmoveEvent(
	const Position& pos, const Move ttm, const History& history, const PieceType pt
)
	: m_pos_(pos), m_history_(history), m_currMove_(GetFirstMove()), m_lastMove_(GetFirstMove())
{
	assert(!pos.InCheck());

	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット
	m_phase_ = N12_ProbCut;

	//m_captureThreshold_ = pos.GetCapturePieceScore(pt);
	m_captureThreshold_ = PieceScore::GetCapturePieceScore(pt);
	m_ttMove_ = ((!ttm.IsNone() &&
		(
			pos.GetTurn()==Color::Black
			?
			pos.MoveIsPseudoLegal<Color::Black,Color::White>(ttm)
			:
			pos.MoveIsPseudoLegal<Color::White,Color::Black>(ttm)
			)		
		) ? ttm : g_MOVE_NONE);

	if (!m_ttMove_.IsNone() && (!m_ttMove_.IsCapture() ||
		(
			pos.GetTurn() == Color::Black
			?
			pos.GetSee1<Color::Black,Color::White>(m_ttMove_)
			:
			pos.GetSee1<Color::White,Color::Black>(m_ttMove_)
		)
		<= m_captureThreshold_)) {
		m_ttMove_ = g_MOVE_NONE;
	}

	m_lastMove_ += !m_ttMove_.IsNone();
}

Move NextmoveEvent::GetNextMove_SplitedNode() {
	// 分岐点の次のノード☆？（＾ｑ＾）？
	return this->m_pFlashlightBox_->m_splitedNode->m_pNextmoveEvent->GetNextMove_NonSplitedNode();
}
Move NextmoveEvent::GetNextMove_NonSplitedNode() {
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

const ScoreIndex LVATable[g_PIECETYPE_NUM] = {
	ScoreIndex(0),
	ScoreIndex(1),
	ScoreIndex(2),
	ScoreIndex(3),
	ScoreIndex(4),
	ScoreIndex(7),
	ScoreIndex(8),
	ScoreIndex(6),
	ScoreIndex(10000),
	ScoreIndex(5),
	ScoreIndex(5),
	ScoreIndex(5),
	ScoreIndex(5),
	ScoreIndex(9),
	ScoreIndex(10)
};
inline ScoreIndex LVA(const PieceType pt) { return LVATable[pt]; }

void NextmoveEvent::ScoreCaptures() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->m_move;
		curr->m_score = PieceScore::GetPieceScore(GetPos().GetPiece(move.To())) - LVA(move.GetPieceTypeFrom());
	}
}

void NextmoveEvent::ScoreEvasions() {
	for (MoveStack* curr = GetCurrMove(); curr != GetLastMove(); ++curr) {
		const Move move = curr->m_move;
		const ScoreIndex seeScore = GetPos().GetSeeSign(move);
		if (seeScore < 0) {
			curr->m_score = seeScore - History::m_MaxScore;
		}
		else if (move.IsCaptureOrPromotion()) {
			curr->m_score = PieceScore::GetCapturePieceScore(GetPos().GetPiece(move.To())) + History::m_MaxScore;
			if (move.IsPromotion()) {
				const PieceType pt = ConvPiece::TO_PIECE_TYPE10(GetPos().GetPiece(move.From()));
				curr->m_score += PieceScore::GetPromotePieceScore(pt);
			}
		}
		else {
			curr->m_score = GetHistory().GetValue(move.IsDrop(), ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(GetPos().GetTurn(), move.GetPieceTypeFromOrDropped()), move.To());
		}
	}
}

void NextmoveEvent::GoNextPhase() {
	m_currMove_ = GetFirstMove(); // legalMoves_[0] は番兵
	++m_phase_;

	g_movePhaseArray[GetPhase()]->GoNext2Phase(*this);
}
