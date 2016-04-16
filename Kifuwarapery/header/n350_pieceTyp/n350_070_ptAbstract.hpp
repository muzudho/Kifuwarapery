#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_650_position.hpp"


class PtAbstract {
public:

	virtual inline PieceType GetNumber() const {
		UNREACHABLE;
	};// = 0;

	virtual inline Bitboard GetAttacks2From(const Bitboard& occupied, const Color c, const Square sq) const {
		UNREACHABLE;
	};// = 0;

	// pin は省かない。
	virtual FORCE_INLINE void Generate2RecaptureMoves(
		MoveStack* moveStackList,
		const Position& pos,
		const Square from,
		const Square to,
		const Color us
		) const {
		UNREACHABLE;
	};// = 0;


};
