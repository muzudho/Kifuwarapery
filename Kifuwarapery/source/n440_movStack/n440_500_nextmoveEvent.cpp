#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_420_convMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_060_stats.hpp"
#include "../../header/n223_move____/n223_500_flashlight.hpp"

#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_030_makePromoteMove.hpp"
#include "../../header/n300_moveGen_/n300_700_moveGen_/n300_700_900_moveList.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"

#include "../../header/n450_movPhase/n450_100_N00_mainSearch.hpp"
#include "../../header/n450_movPhase/n450_110_N01_phTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_120_N02_phKillers.hpp"
#include "../../header/n450_movPhase/n450_130_N03_phNonTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_140_N04_phNonTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_150_N05_phBadCaptures.hpp"
#include "../../header/n450_movPhase/n450_160_N06_evasionSearch.hpp"
#include "../../header/n450_movPhase/n450_170_N07_phEvasions.hpp"
#include "../../header/n450_movPhase/n450_180_N08_qSearch.hpp"
#include "../../header/n450_movPhase/n450_190_N09_phQCaptures0.hpp"
#include "../../header/n450_movPhase/n450_200_N10_qEvasionSearch.hpp"
#include "../../header/n450_movPhase/n450_210_N11_phQEvasions.hpp"
#include "../../header/n450_movPhase/n450_220_N12_probCut.hpp"
#include "../../header/n450_movPhase/n450_230_N13_phTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_240_N14_qRecapture.hpp"
#include "../../header/n450_movPhase/n450_250_N15_phQCaptures1.hpp"
#include "../../header/n450_movPhase/n450_260_N16_phStop.hpp"
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

	this->SetTalonLastCard(this->GetTalonFirstCard());//this->m_lastMove_ =
	this->m_currMove_ = this->GetTalonFirstCard();
	this->SetCaptureThreshold( 0);

	// 山札の許容量の最後の要素にカーソルを合わせる☆？
	this->SetEndBadCaptures( this->GetTalonZeroCard() + Move::m_MAX_LEGAL_MOVES - 1);

	this->m_pFlashlightBox_ = pFlashlightBox;

	if (pos.InCheck()) { // 王手されているなら☆？
		// 最初に、王手回避の手のフェーズへ☆
		this->m_phase_ = N06s_P1__End____EvasionSearch;
	}
	else {
		// まず最初は「メイン・サーチ」フェーズ☆（＾ｑ＾）
		this->m_phase_ = N00s_UNR_End____MainSearch;

		// キラー・ムーブって何だぜ☆？（＾ｑ＾）
		this->m_killerMoves_[0].m_move = pFlashlightBox->m_killers[0];
		this->m_killerMoves_[1].m_move = pFlashlightBox->m_killers[1];

		if (
			this->m_pFlashlightBox_ != nullptr
			&&
			this->m_pFlashlightBox_->m_staticEval < beta - PieceScore::m_capturePawn
			&&
			depth < 3 * OnePly
		) {
			this->SetCaptureThreshold( -PieceScore::m_capturePawn);
		}
		else if (
			m_pFlashlightBox_ != nullptr
			&&
			beta < this->m_pFlashlightBox_->m_staticEval
		) {
			this->SetCaptureThreshold( beta - this->m_pFlashlightBox_->m_staticEval);
		}
	}

	this->m_ttMove_ = (
		// トランスポジション・テーブル・ムーブが設定ありで
		!ttm.IsNone()
		&&
		(
			pos.GetTurn() == Color::Black
			?
			// 先手で
			pos.MoveIsPseudoLegal<Color::Black,Color::White>(ttm)
			:
			// 後手で
			pos.MoveIsPseudoLegal<Color::White,Color::Black>(ttm)
		)		
		?
		// リーガルなら
		ttm
		:
		// イリーガルなら
		g_MOVE_NONE
		);

	// 値が入っていれば１進める☆？
	this->GotoNextTalonLastCard( (!this->m_ttMove_.IsNone()));//>m_lastMove_ +=
}

// 静止探索で呼ばれるコンストラクター。
NextmoveEvent::NextmoveEvent(
	const Position& pos,
	Move ttm,
	const Depth depth,
	const History& history,
	const Square sq
)
	: m_pos_(pos), m_history_(history), m_currMove_(GetTalonFirstCard()), m_lastMove_(GetTalonFirstCard())
{
	assert(depth <= Depth0);

	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット

	if (pos.InCheck())// どれかの駒に、王手されているのかだぜ☆？（＾ｑ＾）？
	{
		// じゃあ、回避フェーズに行こうかだぜ☆？
		m_phase_ = N10s_P1__End___QEvasionSearch;
		// todo: ここで Stockfish は qcheck がある。
	}
	else if (DepthQRecaptures < depth) // たたき合いを読む予定の深さを　超えたのかだぜ☆？（＾ｑ＾）？
	{
		// 素早い検索なのかだぜ☆？
		m_phase_ = N08s_P1__End___QSearch;
	}
	else
	{
		m_phase_ = N14s_P1__UNR___QRecapture;
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
	this->GotoNextTalonLastCard( !m_ttMove_.IsNone());//m_lastMove_ +=
}

// コンストラクター。
NextmoveEvent::NextmoveEvent(
	const Position& pos, const Move ttm, const History& history, const PieceType pt
)
	: m_pos_(pos), m_history_(history), m_currMove_(GetTalonFirstCard()), m_lastMove_(GetTalonFirstCard())
{
	assert(!pos.InCheck());

	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット

	m_phase_ = N12s_P1__End____ProbCut;

	//m_captureThreshold_ = pos.GetCapturePieceScore(pt);
	this->SetCaptureThreshold( PieceScore::GetCapturePieceScore(pt));
	m_ttMove_ = ((!ttm.IsNone() &&
		(
			pos.GetTurn()==Color::Black
			?
			pos.MoveIsPseudoLegal<Color::Black,Color::White>(ttm)
			:
			pos.MoveIsPseudoLegal<Color::White,Color::Black>(ttm)
			)		
		) ? ttm : g_MOVE_NONE);

	if (
		!m_ttMove_.IsNone()
		&&
		(
			!m_ttMove_.IsCapture()
			||
			(
				pos.GetTurn() == Color::Black
				?
				pos.GetSee1<Color::Black,Color::White>(m_ttMove_)
				:
				pos.GetSee1<Color::White,Color::Black>(m_ttMove_)
			) <= this->GetCaptureThreshold()
		)
	) {
		m_ttMove_ = g_MOVE_NONE;
	}

	this->GotoNextTalonLastCard( !m_ttMove_.IsNone());//m_lastMove_ +=
}

Move NextmoveEvent::GetNextMove_SplitedNode() {
	// 分岐点の次のノード☆？（＾ｑ＾）？
	return this->m_pFlashlightBox_->m_splitedNode->m_pNextmoveEvent->GetNextMove_NonSplitedNode();
}
// NextmoveEvent::GetNextMove_SplitedNode() や、DoStep9(...) 、
// HitchhikerQsearchAbstract::DoQsearch(...) 、
// GetNextMove_AtStep11() で使われるぜ☆
Move NextmoveEvent::GetNextMove_NonSplitedNode() {

	// 無限ループ☆　フェーズ１６に達すると g_Move_None が帰ってきて終了☆
	do {
		// このカードで終わり、と決めておいたカードに
		// 達している間、次々と、フェーズを飛ばして進みます。
		while (this->GetCurrCard() == this->GetTalonLastCard()) {

			// 山札の底（最初）のカードに、カーソルを合わせます。
			this->m_currMove_ = this->GetTalonFirstCard(); // legalMoves_[0] は番兵

														   // フェーズを１個進めるぜ☆（＾ｑ＾）
			++this->m_phase_;

			g_movePhaseArray[this->GetPhase()]->Do02_ExtendTalon(*this);
		}

		Move pickedCard;
		bool isGotNext = g_movePhaseArray[this->GetPhase()]->Do03_PickCard_OrNextCard(pickedCard, *this);

		if (isGotNext) {
			return pickedCard;
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
	for (MoveStack* curr = GetCurrCard(); curr != GetTalonLastCard(); ++curr) {
		const Move move = curr->m_move;
		curr->m_score = PieceScore::GetPieceScore(GetPos().GetPiece(move.To())) - LVA(move.GetPieceTypeFrom());
	}
}

void NextmoveEvent::ScoreEvasions() {
	for (MoveStack* curr = GetCurrCard(); curr != GetTalonLastCard(); ++curr) {
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
