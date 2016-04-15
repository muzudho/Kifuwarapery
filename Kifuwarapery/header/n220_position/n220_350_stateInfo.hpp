#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n220_position/n220_300_stateInfoMin.hpp"


// StateInfoMin だけ memcpy でコピーすることああるので、
// 継承を使っている。
struct StateInfo : public StateInfoMin {
public://(^q^)修飾子追加☆m_checkersBBを公開したいんだぜ☆
	Key m_boardKey;
	Key m_handKey;
	// 手番側の玉へ check している駒の Bitboard
	Bitboard m_checkersBB;
	StateInfo* m_previous;
	// 手番側の持ち駒
	Hand m_hand;
	// capturedPieceType は move.cap() で取得出来るので必要無い。
	ChangedLists m_cl;

	Key GetKey() const { return m_boardKey + m_handKey; }
};
