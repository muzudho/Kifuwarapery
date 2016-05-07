#pragma once


#include "../../n105_color___/n105_100_color.hpp"
#include "../../n110_square__/n110_100_square.hpp"
#include "../../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../n160_board___/n160_100_bitboard.hpp"
#include "../../n160_board___/n160_230_setMaskBb.hpp"
#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n165_movStack/n165_420_convMove.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "n300_200_020_moveStack.hpp"
#include "n300_200_040_ptEvent.hpp"
#include "n300_200_045_pieceTypeSeeEvent.hpp"
#include "n300_200_070_ptAbstract.hpp"


const static Move g_PTPROMOTE_AS_MOVE = ConvMove::FROM_PIECETYPE_ONBOARD10(PieceType::PTPromote);


// PieceType::PTPromote;
class PtPromote : public PtAbstract {
public:

	virtual Move AsMove() const override {
		return g_PTPROMOTE_AS_MOVE;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const override {
		return g_nullBitboard;
	}

	// pin ÇÕè»Ç©Ç»Ç¢ÅB//FORCE_INLINE
	void Generate2RecaptureMoves_usWhite(
		MoveStack* pMoveStackList,
		const Position& pos,
		const Square from,
		const Square to
		) const override {
		return;
	}
	void Generate2RecaptureMoves_usBlack(
		MoveStack* pMoveStackList,
		const Position& pos,
		const Square from,
		const Square to
		) const override {
		return;
	}

	PieceType AppendToNextAttackerAndTryPromote(
		Bitboard& occupied,
		Bitboard& attackers,
		PieceType nextPT,
		const PieceTypeSeeEvent ptsEvent
		) const override {
		PieceType PT = PieceType::PTPromote;

		UNREACHABLE;

		// ÇªÇÍà»äOÇÃãÓéÌóﬁÇÕÅAÇªÇÃÇ‹Ç‹ï‘Ç∑Åô
		return PT;
	}

};
