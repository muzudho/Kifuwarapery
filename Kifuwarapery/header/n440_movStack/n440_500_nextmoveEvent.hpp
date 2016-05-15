#pragma once


#include "../n113_piece___/n113_155_convPiece.hpp"
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_420_convMove.hpp"
#include "../n220_position/n220_750_charToPieceUSI.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n300_moveGen_/n300_100_movPhase/n300_100_050_movegeneratorPhase.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../n440_movStack/n440_060_stats.hpp"


using History = Stats<false>;


class NextmoveEvent {
public:

	NextmoveEvent(
		const Position& pos,
		const Move ttm,
		const Depth depth,
		const History& history,
		Flashlight* pFlashlightBox,
		const ScoreIndex beta
	);

	NextmoveEvent(const Position& pos, Move ttm, const Depth depth, const History& history, const Square sq);

	NextmoveEvent(const Position& pos, const Move ttm, const History& history, const PieceType pt);

	Move GetNextMove_SplitedNode();
	Move GetNextMove_NonSplitedNode();


	//────────────────────────────────────────────────────────────────────────────────
	// トランスポジション・テーブル・ムーブ
	//────────────────────────────────────────────────────────────────────────────────
	inline Move GetTranspositionTableMove() const {
		return this->m_ttMove_;
	}
	inline void SetTranspositionTableMove(Move value) {
		this->m_ttMove_ = value;
	}
	inline void SetTranspositionTableMove( Move ttm, const Position& pos) {
		this->m_ttMove_ =
			// トランスポジション・テーブル・ムーブが設定ありで
			!ttm.IsNone()
			&&
			(
				pos.GetTurn() == Color::Black
				?
				// 先手で
				pos.MoveIsPseudoLegal<Color::Black, Color::White>(ttm)
				:
				// 後手で
				pos.MoveIsPseudoLegal<Color::White, Color::Black>(ttm)
				)
			?
			// リーガルなら、引数で渡されてくるムーブを、トランスポジション・テーブル・ムーブとします。
			ttm
			:
			// イリーガルなら
			g_MOVE_NONE;
	}
	inline void ClearTranspositionTableMove() {
		this->SetTranspositionTableMove(g_MOVE_NONE);
	}


	//────────────────────────────────────────────────────────────────────────────────
	// 山札上のカーソル
	//────────────────────────────────────────────────────────────────────────────────
	MoveStack* GetCurrCard() const { return this->m_currMove_; }
	void SetCurrCard(MoveStack* value) { this->m_currMove_ = value; }
	void BackToHome_CurrCard() { this->SetCurrCard(this->GetTalonFirstCard()); };// legalMoves_[0] は番兵
	void GoToTopTalon_CurrCard() { this->SetCurrCard(this->GetSeekbarTerminated()); };// 山札の最後へカーソル移動☆
	void GoToEndTalonCapacity_CurCard(){ this->SetCurrCard(this->GetTalonZeroCard() + Move::m_MAX_LEGAL_MOVES - 1); }// 山札の限界ＭＡＸ頂点（空間の最後）にカーソルを合わせる☆
	inline void GoNextCurCard() {
		++this->m_currMove_;
	};
	inline void GoPreviousCurCard() {
		--this->m_currMove_;
	};


	//────────────────────────────────────────────────────────────────────────────────
	// 山札の一番底（先頭）の１つ前（番兵）のカード
	//────────────────────────────────────────────────────────────────────────────────
	// 旧名：ＧｅｔＬｅｇａｌＭｏｖｅｓ
	// カードを取得。
	MoveStack* GetTalonZeroCard() { return this->m_legalMoves_; }

	//────────────────────────────────────────────────────────────────────────────────
	// 山札の一番底（先頭）のカード
	//────────────────────────────────────────────────────────────────────────────────
	// カードを取得。
	MoveStack* GetTalonFirstCard() { return &m_legalMoves_[1]; } // [0] は番兵

	//────────────────────────────────────────────────────────────────────────────────
	// シーク・バーの終着点とするカード
	//────────────────────────────────────────────────────────────────────────────────
	// 山札の一番上（最後）だったり、天札の底（最後）だったりするぜ☆（＾ｑ＾）
	MoveStack* GetSeekbarTerminated() const { return this->m_lastMove_; }
	// 山札の一番上（最後）のカードを覚えておきます。
	void SetSeekbarTerminated(MoveStack* value) { this->m_lastMove_ = value; }
	void IncreaseSeekbarTerminated(int offset) { this->m_lastMove_ += offset; }

	//────────────────────────────────────────────────────────────────────────────────
	// 駒を取らない手の最後のカード
	//────────────────────────────────────────────────────────────────────────────────
	// 駒を取らない手の最後のカードを覚えておきます。
	MoveStack* Get_LastNonCapture() const { return m_lastNonCapture_; }
	void SetLastNonCaputre(MoveStack* value) { this->m_lastNonCapture_ = value; }


	const Position& GetPos() const { return this->m_pos_; }


	// 駒取りの閾値☆？
	int GetCaptureThreshold() const { return this->m_captureThreshold_; }
	void SetCaptureThreshold(int value) { this->m_captureThreshold_ = value; }


	//────────────────────────────────────────────────────────────────────────────────
	// 天札の底（最後）　（山札の許容空間の最後から、前方に戻ってカーソルが進んでいくぜ☆）
	//────────────────────────────────────────────────────────────────────────────────
	// 旧名：ＥｎｄＢａｄＣａｐｔｕｒｅｓ
	//
	// 貧乏削りの鉛筆みたいだぜ☆（＾ｑ＾）
	// 点数が悪かった駒の取り合いの手を、まだマシな順に後ろから入れていくぜ☆
	MoveStack* GetBottom_SkyTalon() const { return this->m_pEndBadCaptures_; }
	void DownBottom_SkyTalon() { this->m_pEndBadCaptures_--; }
	void SetBottom_SkyTalon(MoveStack* value) { this->m_pEndBadCaptures_ = value; }//ボトムがエンド☆

	// 天札　を消去します。
	void Clear_SkyTalon(){ this->SetBottom_SkyTalon(this->GetTalonZeroCard() + Move::m_MAX_LEGAL_MOVES - 1); }


	//────────────────────────────────────────────────────────────────────────────────
	// キラー・ムーブス
	//────────────────────────────────────────────────────────────────────────────────
	MoveStack* GetKillerMoves() const { return (MoveStack*)this->m_killerMoves_; }
	MoveStack GetKillerMove(int index) const { return this->m_killerMoves_[index]; }



	Square GetRecaptureSquare()const { return this->m_recaptureSquare_; }

	Depth GetDepth() const { return this->m_depth_; }


	//────────────────────────────────────────────────────────────────────────────────
	// フェーズ
	//────────────────────────────────────────────────────────────────────────────────
	void SetPhase(GenerateMovePhase value) { this->m_phase_ = value; }
	void IncreasePhase() { ++this->m_phase_; }


public:// もともと本当はプライベート・メソッド☆

	template <bool IsDrop>
	void NextmoveEvent::DoScoreing_NonCapturesMinusPro() {
		for (MoveStack* curr = this->GetCurrCard(); curr != this->GetSeekbarTerminated(); ++curr) {
			const Move move = curr->m_move;

			curr->m_score =	this->GetHistory().GetValue(
				// ドロップか
				IsDrop,
				// 駒
				ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(
					GetPos().GetTurn(),
					(IsDrop ? move.GetPieceTypeDropped() : move.GetPieceTypeFrom())
				),
				// 移動先
				move.To()
			);
		}
	}

	void DoScoreing_Captures();

	void DoScoreing_Evasions();

private:

	GenerateMovePhase GetPhase() const { return m_phase_; }


	//────────────────────────────────────────────────────────────────────────────────
	// ヒストリー
	//────────────────────────────────────────────────────────────────────────────────
	// 駒を取らない手で見かける☆
	const History& GetHistory() const { return m_history_; }


private:
	const Position&		m_pos_;

	const History&		m_history_;

	// サーチ・スタック☆
	Flashlight*			m_pFlashlightBox_;

	Depth				m_depth_;

	Move				m_ttMove_; // transposition table move

	MoveStack			m_killerMoves_[2];

	Square				m_recaptureSquare_;

	// 駒取りの閾値☆？（＾ｑ＾）？
	int					m_captureThreshold_; // int で良いのか？

	GenerateMovePhase	m_phase_;

	MoveStack*			m_currMove_;

	MoveStack*			m_lastMove_;

	MoveStack*			m_lastNonCapture_;

	MoveStack*			m_pEndBadCaptures_;

	// std::array にした方が良さそう。
	MoveStack			m_legalMoves_[Move::m_MAX_LEGAL_MOVES];

};

