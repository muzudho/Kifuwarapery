#pragma once

#include <algorithm> //std::swap std::max_element
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_105_convPieceType.hpp"
#include "../n165_movStack/n165_400_move.hpp"
#include "../n165_movStack/n165_420_convMove.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n220_position/n220_660_utilMovePos.hpp"


class UtilMove01 {
public:

	// 汎用的な insertion sort. 要素数が少ない時、高速にソートできる。
	// 降順(大きいものが先頭付近に集まる)
	// *(first - 1) に 番兵(sentinel) として MAX 値が入っていると仮定して高速化してある。
	// T には ポインタかイテレータを使用出来る。
	template <typename T, bool UseSentinel = false>
	static inline void InsertionSort(T first, T last) {
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

	static inline Move Move16toMove(const Move move, const Position& pos) {
		if (move.IsNone()) {
			return g_MOVE_NONE;
		}
		if (move.IsDrop()) {
			return move;
		}
		const Square from = move.From();
		const PieceType ptFrom = ConvPiece::TO_PIECE_TYPE10(pos.GetPiece(from));
		return move | ConvMove::FROM_PIECETYPE_ONBOARD10(ptFrom) | UtilMovePos::GET_CAPTURED_PIECE_FROM_SQ(move.To(), pos);
	}


};

