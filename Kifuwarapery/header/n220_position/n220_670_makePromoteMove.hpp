#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n165_movStack/n165_300_moveType.hpp"
#include "../n165_movStack/n165_310_promoteMode.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n220_position/n220_660_utilMovePos.hpp"


class Position;


class MakePromoteMove {
public:

	// MoveType によって指し手生成関数を使い分ける。
	// Drop, Check, Evasion, の場合は別で指し手生成を行う。
	template <PromoteMode PM>
	inline Move GetSelectedMakeMove_mt(MoveType MT, const PieceType pt, const Square from, const Square to, const Position& pos) {
		static_assert(PM == Promote || PM == NonPromote, "");
		assert(!((pt == N07_Gold || pt == N08_King || MT == Drop) && PM == Promote));
		Move move = ((MT == NonCapture || MT == NonCaptureMinusPro) ? UtilMove::MakeMove(pt, from, to) : UtilMovePos::MakeCaptureMove(pt, from, to, pos));
		if (PM == Promote) {
			move |= UtilMove::GetPromoteFlag();
		}
		return move;
	}

	inline Move MakePromoteMove2_mt(MoveType MT, const PieceType pt, const Square from, const Square to, const Position& pos) {
		return GetSelectedMakeMove_mt< Promote>(MT, pt, from, to, pos);
	}

	inline Move MakeNonPromoteMove_mt(MoveType MT, const PieceType pt, const Square from, const Square to, const Position& pos) {
		return GetSelectedMakeMove_mt< NonPromote>(MT, pt, from, to, pos);
	}

};


extern MakePromoteMove g_makePromoteMove;
