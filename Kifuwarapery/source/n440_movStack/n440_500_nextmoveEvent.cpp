#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_420_convMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_500_flashlight.hpp"

#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_030_moveMaker_ExceptPromote.hpp"
#include "../../header/n300_moveGen_/n300_700_moveGen_/n300_700_900_moveList.hpp"
#include "../../header/n440_movStack/n440_060_stats.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"

#include "../../header/n450_movPhase/n450_100_N00_mainSearch.hpp"
#include "../../header/n450_movPhase/n450_110_N01___TacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_120_N02___Killers.hpp"
#include "../../header/n450_movPhase/n450_130_N03___NonTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_140_N04___NonTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_150_N05___BadCaptures.hpp"
#include "../../header/n450_movPhase/n450_160_N06_evasionSearch.hpp"
#include "../../header/n450_movPhase/n450_170_N07___Evasions.hpp"
#include "../../header/n450_movPhase/n450_180_N08_qSearch.hpp"
#include "../../header/n450_movPhase/n450_190_N09___QCaptures0.hpp"
#include "../../header/n450_movPhase/n450_200_N10_qEvasionSearch.hpp"
#include "../../header/n450_movPhase/n450_210_N11___QEvasions.hpp"
#include "../../header/n450_movPhase/n450_220_N12_probCut.hpp"
#include "../../header/n450_movPhase/n450_230_N13___TacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_240_N14_qRecapture.hpp"
#include "../../header/n450_movPhase/n450_250_N15___QCaptures1.hpp"
#include "../../header/n450_movPhase/n450_260_N16___Stop.hpp"
#include "../../header/n450_movPhase/n450_500_movePhaseArray.hpp"


#include "../../header/n640_searcher/n640_440_splitedNode.hpp"	// 持ち合いになっているが .cpp だからいいかだぜ☆（＾ｑ＾）



using History = Stats<false>;


// （＾ｑ＾）元の名前は ＭｏｖｅＰｉｃｋｅｒ☆
// 指し手を選ぶ関数の引数として使われるぜ☆（＾ｑ＾）
// 通常の探索で呼ばれるコンストラクター☆
NextmoveEvent::NextmoveEvent(
	const Position& pos,
	const Move ttm,
	const Depth depth,
	const History& history,
	Flashlight* pFlashlightBox,	// 静止探索でない場合
	const ScoreIndex beta		// 静止探索でない場合
)
	: m_pos_(pos), m_history_(history), m_depth_(depth)
{
	assert(Depth0 < depth);

	this->m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット

	this->SetSeekbarTerminated(this->GetTalonFirstCard());//this->m_lastMove_ =
	this->BackToHome_CurrCard();
	this->SetCaptureThreshold( 0);

	// 山札の許容量の最後の要素にカーソルを合わせる☆？
	this->Clear_SkyTalon();

	this->m_pFlashlightBox_ = pFlashlightBox;

	if (pos.InCheck()) { // 王手されているなら☆？
		// 最初に、王手回避の手のフェーズへ☆
		this->SetPhase( N06_Start_P1__EvasionSearch);
	}
	else {
		// まず最初は「メイン・サーチ」フェーズ☆（＾ｑ＾）
		this->SetPhase( N00_Start_UNR_MainSearch);

		// キラー・ムーブって　栞　かだぜ☆？（＾ｑ＾）
		this->GetKillerMove(0).m_move = pFlashlightBox->m_killers[0];
		this->GetKillerMove(1).m_move = pFlashlightBox->m_killers[1];

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

	this->SetTranspositionTableMove(ttm, pos);

	// 値が入っていれば１進める☆？
	this->IncreaseSeekbarTerminated( (!this->GetTranspositionTableMove().IsNone()));//>m_lastMove_ +=
}

// 静止探索で呼ばれるコンストラクター。
NextmoveEvent::NextmoveEvent(
	const Position& pos,
	Move ttm,
	const Depth depth,
	const History& history,
	const Square sq				// 静止探索
)
	: m_pos_(pos), m_history_(history), m_currMove_(GetTalonFirstCard()), m_lastMove_(GetTalonFirstCard())
{
	assert(depth <= Depth0);

	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット

	if (pos.InCheck())// どれかの駒に、王手されているのかだぜ☆？（＾ｑ＾）？
	{
		// じゃあ、回避フェーズに行こうかだぜ☆？
		this->SetPhase( N10_Start_P1__QEvasionSearch);
		// todo: ここで Stockfish は qcheck がある。
	}
	else if (DepthQRecaptures < depth) // たたき合いを読む予定の深さを　超えたのかだぜ☆？（＾ｑ＾）？
	{
		// 素早い検索なのかだぜ☆？
		this->SetPhase( N08_Start_P1__QSearch);
	}
	else
	{
		this->SetPhase( N14_Start_P1__QRecapture);
		m_recaptureSquare_ = sq;
		ttm = g_MOVE_NONE;
	}

	this->SetTranspositionTableMove(ttm, pos);
	this->IncreaseSeekbarTerminated( ! this->GetTranspositionTableMove().IsNone());//m_lastMove_ +=
}

// コンストラクター。
// その他の探索？
NextmoveEvent::NextmoveEvent(
	const Position& pos,
	const Move ttm,
	const History& history,
	const PieceType pt			// その他の探索の場合
)
	: m_pos_(pos), m_history_(history), m_currMove_(GetTalonFirstCard()), m_lastMove_(GetTalonFirstCard())
{
	assert(!pos.InCheck());

	m_legalMoves_[0].m_score = INT_MAX; // 番兵のセット

	this->SetPhase( N12_Start_P1__ProbCut);

	//m_captureThreshold_ = pos.GetCapturePieceScore(pt);
	this->SetCaptureThreshold( PieceScore::GetCapturePieceScore(pt));
	this->SetTranspositionTableMove(ttm, pos);

	if (
		! this->GetTranspositionTableMove().IsNone()
		&&
		(
			! this->GetTranspositionTableMove().IsCapture()
			||
			(
				pos.GetTurn() == Color::Black
				?
				pos.GetSee1<Color::Black,Color::White>( this->GetTranspositionTableMove())
				:
				pos.GetSee1<Color::White,Color::Black>( this->GetTranspositionTableMove())
			) <= this->GetCaptureThreshold()
		)
	) {
		this->SetTranspositionTableMove( g_MOVE_NONE);
	}

	this->IncreaseSeekbarTerminated( ! this->GetTranspositionTableMove().IsNone());//m_lastMove_ +=
}

Move NextmoveEvent::GetNextMove_SplitedNode() {
	// 分岐点の次のノード☆？（＾ｑ＾）？
	return this->m_pFlashlightBox_->m_splitedNode->m_pVideodeck->GetNextMove_NonSplitedNode();
}
// NextmoveEvent::GetNextMove_SplitedNode() や、DoStep9(...) 、
// HitchhikerQsearchAbstract::DoQsearch(...) 、
// GetNextMove_AtStep11() で使われるぜ☆
Move NextmoveEvent::GetNextMove_NonSplitedNode() {

	// 無限ループ☆　フェーズ１６に達すると g_Move_None が帰ってきて終了☆
	do {
		// このカードで終わり、と決めておいたカードに
		// 達している間、次々と、フェーズを飛ばして進みます。
		while (this->GetCurrCard() == this->GetSeekbarTerminated()) {

			// フェーズを１個進めるぜ☆（＾ｑ＾）
			this->IncreasePhase();

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
	// どの駒で取るか、という順を付けているんじゃないか☆（＾ｑ＾）？
	// 列挙型にキャストしているぜ☆
	ScoreIndex(0),		// オキュパイ
	ScoreIndex(1),		// 歩 （歩で取りに行くのが一番いいぜ☆）
	ScoreIndex(2),		// 香
	ScoreIndex(3),		// 桂
	ScoreIndex(4),		// 銀
	ScoreIndex(7),		// 角
	ScoreIndex(8),		// 飛
	ScoreIndex(6),		// 金
	ScoreIndex(10000),	// 玉 （玉では取りたくない？）
	ScoreIndex(5),		// と
	ScoreIndex(5),		// 杏
	ScoreIndex(5),		// 圭
	ScoreIndex(5),		// 全
	ScoreIndex(9),		// 馬
	ScoreIndex(10)		// 竜
};
inline ScoreIndex LVA(const PieceType pt) { return LVATable[pt]; }

// 駒を取る手に点数付け☆
void NextmoveEvent::DoScoreing_Captures() {
	for (MoveStack* curr = this->GetCurrCard(); curr != this->GetSeekbarTerminated(); ++curr)
	{
		const Move move = curr->m_move;
		// 移動先にある駒（これから取る駒）の点数だぜ☆
		// 移動元の駒の種類に応じて減点して入るのは、歩で取りたい、玉では取りたくない、という差を付けているんじゃないか☆？
		curr->m_score = PieceScore::GetPieceScore(GetPos().GetPiece(move.To())) - LVA(move.GetPieceTypeFrom());
	}
}

void NextmoveEvent::DoScoreing_Evasions() {
	for (MoveStack* curr = GetCurrCard(); curr != GetSeekbarTerminated(); ++curr) {
		const Move move = curr->m_move;
		const ScoreIndex seeScore = GetPos().GetSeeSign(move);
		if (seeScore < 0) {
			curr->m_score = seeScore - History::m_MAX_SCORE;
		}
		else if (move.IsCaptureOrPromotion()) {
			curr->m_score = PieceScore::GetCapturePieceScore(GetPos().GetPiece(move.To())) + History::m_MAX_SCORE;
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
