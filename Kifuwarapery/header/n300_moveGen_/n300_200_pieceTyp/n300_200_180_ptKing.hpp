﻿#pragma once


#include "../../n105_color___/n105_100_color.hpp"
#include "../../n110_square__/n110_100_square.hpp"
#include "../../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../n160_board___/n160_100_bitboard.hpp"
#include "../../n160_board___/n160_160_kingAttackBb.hpp"
#include "../../n160_board___/n160_230_setMaskBb.hpp"
#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n165_movStack/n165_420_convMove.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "n300_200_020_moveStack.hpp"
#include "n300_200_030_moveMaker_ExceptPromote.hpp"
#include "n300_200_040_ptEvent.hpp"
#include "n300_200_045_pieceTypeSeeEvent.hpp"
#include "n300_200_070_ptAbstract.hpp"


const static Move g_PTKING_ONBOARD_AS_MOVE = ConvMove::FROM_PIECETYPE_ONBOARD10(PieceType::N08_King);


// ＰｉｅｃｅＴｙｐｅ：：Ｎ０８＿Ｋｉｎｇ
class PtKing : public PtAbstract {
public:

	virtual Move AsMove() const override {
		return g_PTKING_ONBOARD_AS_MOVE;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const override {
		return g_kingAttackBb.GetControllBb(ptEvent.m_sq);
	}

	// pin は省かない。
	void Generate2RecaptureMoves_usWhite(//FORCE_INLINE
		MoveStack* pMoveStackList,
		const Position& pos,
		const Square from,
		const Square to
		) const override {

		pMoveStackList->m_move = g_makePromoteMove.BuildCard_CaptureCategory(
			pos, this->AsMove(), from, to);

		pMoveStackList++;
	}

	void Generate2RecaptureMoves_usBlack(//FORCE_INLINE
		MoveStack* pMoveStackList,
		const Position& pos,
		const Square from,
		const Square to
		) const override {

		pMoveStackList->m_move = g_makePromoteMove.BuildCard_CaptureCategory(
			pos, this->AsMove(), from, to);

		pMoveStackList++;
	}

	PieceType AppendToNextAttackerAndTryPromote(
		Bitboard& occupied,
		Bitboard& attackers,
		PieceType nextPT,
		const PieceTypeSeeEvent ptsEvent
		) const override;

};
