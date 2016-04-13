#pragma once

#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_750_charToPieceUSI.hpp"
#include "../n225_movPhase/n225_050_generateMovePhase.hpp"
#include "../n223_move____/n223_060_stats.hpp"
#include "../n223_move____/n223_500_searchStack.hpp"


using History = Stats<false>;


class MovePicker {
public:

	MovePicker(
		const Position& pos,
		const Move ttm,
		const Depth depth,
		const History& history,
		SearchStack* searchStack,
		const Score beta
	);

	MovePicker(const Position& pos, Move ttm, const Depth depth, const History& history, const Square sq);

	MovePicker(const Position& pos, const Move ttm, const History& history, const PieceType pt);

	template <bool SPNODE>
	Move GetNextMove();

	inline void IncrementCurMove() {
		++this->m_currMove_;
	};

	inline void DecrementCurMove() {
		--this->m_currMove_;
	};

	inline Move GetTranspositionTableMove() {
		return this->m_ttMove_;
	}

	MoveStack* GetCurrMove() const { return this->m_currMove_; }

	MoveStack* GetLastMove() const { return this->m_lastMove_; }

	const Position& GetPos() const { return this->m_pos_; }

	int GetCaptureThreshold() const { return this->m_captureThreshold_; }

	MoveStack* GetEndBadCaptures() const { return this->m_endBadCaptures_; }
	void DecrementEndBadCaptures() { this->m_endBadCaptures_--; }

	MoveStack GetKillerMove(int index) const { return this->m_killerMoves_[index]; }

	Square GetRecaptureSquare()const { return this->m_recaptureSquare_; }

private:

	void ScoreCaptures();

	template <bool IsDrop>
	void ScoreNonCapturesMinusPro();

	void ScoreEvasions();

	void GoNextPhase();

	MoveStack* GetFirstMove() { return &m_legalMoves_[1]; } // [0] は番兵

	MoveStack* GetLastNonCapture() const { return m_lastNonCapture_; }

	GenerateMovePhase GetPhase() const { return m_phase_; }

	const History& GetHistory() const { return m_history_; }


	const Position&		m_pos_;

	const History&		m_history_;

	SearchStack*		m_ss_;

	Depth				m_depth_;

	Move				m_ttMove_; // transposition table move

	MoveStack			m_killerMoves_[2];

	Square				m_recaptureSquare_;

	int					m_captureThreshold_; // int で良いのか？

	GenerateMovePhase	m_phase_;

	MoveStack*			m_currMove_;

	MoveStack*			m_lastMove_;

	MoveStack*			m_lastNonCapture_;

	MoveStack*			m_endBadCaptures_;

	// std::array にした方が良さそう。
	MoveStack			m_legalMoves_[g_MaxLegalMoves];

};

