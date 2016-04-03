#pragma once

#include "n240_150_move.hpp"


struct MoveStack {
	Move move;
	int score;
};

// insertionSort() や std::sort() で必要
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.score < s.score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.score > s.score; }

// 汎用的な insertion sort. 要素数が少ない時、高速にソートできる。
// 降順(大きいものが先頭付近に集まる)
// *(first - 1) に 番兵(sentinel) として MAX 値が入っていると仮定して高速化してある。
// T には ポインタかイテレータを使用出来る。
template <typename T, bool UseSentinel = false> inline void insertionSort(T first, T last) {
	if (UseSentinel) {
		assert(std::max_element(first - 1, last) == first - 1); // 番兵が最大値となることを確認
	}
	if (first != last) {
		for (T curr = first + 1; curr != last; ++curr) {
			if (*(curr - 1) < *curr) {
				const auto tmp = std::move(*curr);
				do {
					*curr = *(curr - 1);
					--curr;
				} while ((UseSentinel || curr != first)
					&& *(curr - 1) < tmp);
				*curr = std::move(tmp);
			}
		}
	}
}

// 最も score の高い moveStack のポインタを返す。
// MoveStack の数が多いとかなり時間がかかるので、
// 駒打ちを含むときに使用してはならない。
inline MoveStack* pickBest(MoveStack* currMove, MoveStack* lastMove) {
	std::swap(*currMove, *std::max_element(currMove, lastMove));
	return currMove;
}

inline Move move16toMove(const Move move, const Position& pos) {
	if (move.isNone()) {
		return Move::moveNone();
	}
	if (move.isDrop()) {
		return move;
	}
	const Square from = move.from();
	const PieceType ptFrom = UtilPiece::ToPieceType(pos.piece(from));
	return move | pieceType2Move(ptFrom) | capturedPieceType2Move(move.to(), pos);
}

