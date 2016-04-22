#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_670_makePromoteMove.hpp"
#include "n350_040_ptEvent.hpp"
#include "n350_070_ptAbstract.hpp"


class PtOccupied : public PtAbstract {
public:

	PieceType GetNumber() const {
		return PieceType::N00_Occupied;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const {
		return Bitboard::CreateAllZeroBB();
	}

	void Generate2RecaptureMoves(//FORCE_INLINE
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
	) const {
		// 最適化の為のダミー
		assert(false);
	}

	Bitboard AppendToNextAttacker(
		Bitboard& attackers,
		const Position& pos,
		const Square to,
		Bitboard& occupied,
		const Color turn
	) const {
		return attackers;
	}

	PieceType TryPromoteNextAttacker(
		const PieceType PT,
		const Square to,
		const Color turn,
		const Square from
	) const {
		// それ以外の駒種類は、そのまま返す☆
		return PT;
	}

};


extern PtOccupied g_ptOccupied;
