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
		assert(!((pt == N07_Gold || pt == N08_King || mt == Drop) && pm == Promote));
		Move move = ((mt == NonCapture || mt == NonCaptureMinusPro) ? UtilMove::MakeMove(pt, from, to) : UtilMovePos::MakeCaptureMove(pt, from, to, pos));
		if (pm == Promote) {
			move |= UtilMove::GetPromoteFlag();
		}
		return move;
	}

	//(^q^)旧型
	// MoveType によって指し手生成関数を使い分ける。
	// Drop, Check, Evasion, の場合は別で指し手生成を行う。
	template <MoveType MT, PromoteMode PM>
	inline Move GetSelectedMakeMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
		static_assert(PM == Promote || PM == NonPromote, "");
		assert(!((pt == N07_Gold || pt == N08_King || MT == Drop) && PM == Promote));
		Move move = ((MT == NonCapture || MT == NonCaptureMinusPro) ? UtilMove::MakeMove(pt, from, to) : UtilMovePos::MakeCaptureMove(pt, from, to, pos));
		if (PM == Promote) {
			move |= UtilMove::GetPromoteFlag();
		}
		return move;
	}

	// (^q^)新型
	static inline Move MakePromoteMove2(const MoveType mt, const PieceType pt, const Square from, const Square to, const Position& pos) {
		return GetSelectedMakeMove(mt, Promote, pt, from, to, pos);
	}

	// (^q^)旧型
	template <MoveType MT>
	inline Move MakePromoteMove2(const PieceType pt, const Square from, const Square to, const Position& pos) {
		return GetSelectedMakeMove<MT, Promote>(pt, from, to, pos);
	}

	// (^q^)新型
	static inline Move MakeNonPromoteMove(const MoveType mt, const PieceType pt, const Square from, const Square to, const Position& pos) {
		return GetSelectedMakeMove(mt, NonPromote, pt, from, to, pos);
	}

	// (^q^)旧型
	template <MoveType MT>
	inline Move MakeNonPromoteMove(const PieceType pt, const Square from, const Square to, const Position& pos) {
		return GetSelectedMakeMove<MT, NonPromote>(pt, from, to, pos);
	}

};


extern MakePromoteMove g_makePromoteMove;
