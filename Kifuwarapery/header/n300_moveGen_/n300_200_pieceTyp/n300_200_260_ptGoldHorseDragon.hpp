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
#include "n300_200_030_makePromoteMove.hpp"
#include "n300_200_040_ptEvent.hpp"
#include "n300_200_045_pieceTypeSeeEvent.hpp"
#include "n300_200_070_ptAbstract.hpp"


// PieceType::N16_GoldHorseDragon
class PtGoldHorseDragon : public PtAbstract {
public:

	virtual Move AsMove() const override {
		UNREACHABLE;
		return g_MOVE_NONE;// Ç±ÇÍÇÕégÇ¡ÇƒÇÕÇ¢ÇØÇ»Ç¢ÅB
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const override {
		return g_nullBitboard;
	}

	// pin ÇÕè»Ç©Ç»Ç¢ÅB
	void Generate2RecaptureMoves_usWhite(//FORCE_INLINE
		MoveStack* pMoveStackList,
		const Position& pos,
		const Square from,
		const Square to
		) const override {
		return;
	}

	void Generate2RecaptureMoves_usBlack(//FORCE_INLINE
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
		) const override;

};
