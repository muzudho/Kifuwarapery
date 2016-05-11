#pragma once


#include "../n113_piece___/n113_155_convPiece.hpp"
#include "../n119_score___/n119_090_scoreIndex.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_420_convMove.hpp"
#include "../n220_position/n220_750_charToPieceUSI.hpp"
#include "../n223_move____/n223_060_stats.hpp"
#include "../n223_move____/n223_500_flashlight.hpp"
#include "../n300_moveGen_/n300_100_movPhase/n300_100_050_movegeneratorPhase.hpp"
#include "../n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"


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

	inline Move GetTranspositionTableMove() {
		return this->m_ttMove_;
	}

	MoveStack* GetCurrCard() const { return this->m_currMove_; }
	void SetCurrCard(MoveStack* value) { this->m_currMove_ = value; }
	inline void GoNextCurCard() {
		++this->m_currMove_;
	};
	inline void GoPreviousCurCard() {
		--this->m_currMove_;
	};


	// 旧名：ＧｅｔＬｅｇａｌＭｏｖｅｓ
	// 山札の一番底（先頭）の１つ前（番兵）のカードを取得。
	MoveStack* GetTalonZeroCard() { return this->m_legalMoves_; }

	// 山札の一番底（先頭）のカードを取得。
	MoveStack* GetTalonFirstCard() { return &m_legalMoves_[1]; } // [0] は番兵

	MoveStack* GetTalonLastCard() const { return this->m_lastMove_; }
	// 山札の一番上（最後）のカードを覚えておきます。
	void SetTalonLastCard(MoveStack* value) { this->m_lastMove_ = value; }
	void GotoNextTalonLastCard(int offset) { this->m_lastMove_ += offset; }
	void SetTalonLastCardAndLastNonCaputre(MoveStack* value) {
		this->m_lastMove_ = value;
		this->m_lastNonCapture_ = value;
	}
	MoveStack* GetLastNonCapture() const { return m_lastNonCapture_; }


	const Position& GetPos() const { return this->m_pos_; }


	// 駒取りの閾値☆？
	int GetCaptureThreshold() const { return this->m_captureThreshold_; }
	void SetCaptureThreshold(int value) { this->m_captureThreshold_ = value; }


	MoveStack* GetEndBadCaptures() const { return this->m_pEndBadCaptures_; }
	void GoToPreviousEndBadCaptures() { this->m_pEndBadCaptures_--; }
	void SetEndBadCaptures(MoveStack* value) { this->m_pEndBadCaptures_ = value; }

	MoveStack* GetKillerMoves() const { return (MoveStack*)this->m_killerMoves_; }
	MoveStack GetKillerMove(int index) const { return this->m_killerMoves_[index]; }

	Square GetRecaptureSquare()const { return this->m_recaptureSquare_; }

	Depth GetDepth() const { return this->m_depth_; }

	void SetPhase(GenerateMovePhase value) { this->m_phase_ = value; }


public:// もともと本当はプライベート・メソッド☆

	//template <bool IsDrop> void ScoreNonCapturesMinusPro();
	template <bool IsDrop>
	void NextmoveEvent::ScoreNonCapturesMinusPro() {
		for (MoveStack* curr = this->GetCurrCard(); curr != this->GetTalonLastCard(); ++curr) {
			const Move move = curr->m_move;
			curr->m_score =
				GetHistory().GetValue(IsDrop,
					ConvPiece::FROM_COLOR_AND_PIECE_TYPE10(GetPos().GetTurn(),
						(IsDrop ? move.GetPieceTypeDropped() : move.GetPieceTypeFrom())),
					move.To());
		}
	}

	void ScoreCaptures();

	void ScoreEvasions();

private:

	GenerateMovePhase GetPhase() const { return m_phase_; }

	const History& GetHistory() const { return m_history_; }


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

