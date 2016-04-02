#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n120_brdEntry/n120_200_piece.hpp"
#include "../n240_position/n240_150_move.hpp"
#include "../n276_genMove_/n276_200_promoteMode.hpp"


// MoveType によって指し手生成関数を使い分ける。
// Drop, Check, Evasion, の場合は別で指し手生成を行う。
template <MoveType MT, PromoteMode PM>
inline Move selectedMakeMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
	static_assert(PM == Promote || PM == NonPromote, "");
	assert(!((pt == Gold || pt == King || MT == Drop) && PM == Promote));
	Move move = ((MT == NonCapture || MT == NonCaptureMinusPro) ? makeMove(pt, from, to) : makeCaptureMove(pt, from, to, pos));
	if (PM == Promote) {
		move |= promoteFlag();
	}
	return move;
}

template <MoveType MT>
inline Move makePromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
	return selectedMakeMove<MT, Promote>(pt, from, to, pos);
}

template <MoveType MT>
inline Move makeNonPromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
	return selectedMakeMove<MT, NonPromote>(pt, from, to, pos);
}
