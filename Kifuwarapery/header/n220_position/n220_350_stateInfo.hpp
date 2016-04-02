#pragma once

#include "../../header/n220_position/n220_300_stateInfoMin.hpp"


// StateInfoMin だけ memcpy でコピーすることああるので、
// 継承を使っている。
struct StateInfo : public StateInfoMin {
	Key boardKey;
	Key handKey;
	// 手番側の玉へ check している駒の Bitboard
	Bitboard checkersBB;
	StateInfo* previous;
	// 手番側の持ち駒
	Hand hand;
	// capturedPieceType は move.cap() で取得出来るので必要無い。
	ChangedLists cl;

	Key key() const { return boardKey + handKey; }
};
