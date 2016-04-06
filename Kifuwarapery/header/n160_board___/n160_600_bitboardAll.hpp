#pragma once

#include "../n160_board___/n160_500_bitboardMask.hpp"


// Bitboard の全ての bit に対して同様の処理を行う際に使用するマクロ
// xxx に処理を書く。
// xxx には template 引数を 2 つ以上持つクラスや関数は () でくくらないと使えない。
// これはマクロの制約。
// 同じ処理のコードが 2 箇所で生成されるため、コードサイズが膨れ上がる。
// その為、あまり多用すべきでないかも知れない。
#define FOREACH_BB(bb, sq, xxx)					\
	do {										\
		while (bb.GetP(0)) {					\
			sq = bb.FirstOneRightFromI9();		\
			xxx;								\
		}										\
		while (bb.GetP(1)) {					\
			sq = bb.FirstOneLeftFromB9();		\
			xxx;								\
		}										\
	} while (false)

template <typename T> FORCE_INLINE void foreachBB(Bitboard& bb, Square& sq, T t) {
	while (bb.GetP(0)) {
		sq = bb.FirstOneRightFromI9();
		t(0);
	}
	while (bb.GetP(1)) {
		sq = bb.FirstOneLeftFromB9();
		t(1);
	}
}
