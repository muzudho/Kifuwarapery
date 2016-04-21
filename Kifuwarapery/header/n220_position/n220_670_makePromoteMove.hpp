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

	//(^q^)新型
	// MoveType によって指し手生成関数を使い分ける。
	// Drop, Check, Evasion, の場合は別で指し手生成を行う。
	static inline Move GetSelectedMakeMove(MoveType mt, PromoteMode pm, const PieceType pt, const Square from, const Square to, const Position& pos) {
		assert(pm == Promote || pm == NonPromote, "");
		assert(!((pt == N07_Gold || pt == N08_King || mt == N02_Drop) && pm == Promote));
		Move move = ((mt == N01_NonCapture || mt == N04_NonCaptureMinusPro) ?
			UtilMove::MakeMove(pt, from, to) :
			UtilMovePos::MakeCaptureMove(pt, from, to, pos)
		);
		if (pm == Promote) {
			move |= g_MOVE_PROMOTE_FLAG;// UtilMove::GetPromoteFlag()
		}
		return move;
	}

};


extern MakePromoteMove g_makePromoteMove;
