#pragma once

#include "n240_150_move.hpp"


struct MoveStack {
	Move move;
	int score;
};

// insertionSort() �� std::sort() �ŕK�v
inline bool operator < (const MoveStack& f, const MoveStack& s) { return f.score < s.score; }
inline bool operator > (const MoveStack& f, const MoveStack& s) { return f.score > s.score; }

// �ėp�I�� insertion sort. �v�f�������Ȃ����A�����Ƀ\�[�g�ł���B
// �~��(�傫�����̂��擪�t�߂ɏW�܂�)
// *(first - 1) �� �ԕ�(sentinel) �Ƃ��� MAX �l�������Ă���Ɖ��肵�č��������Ă���B
// T �ɂ� �|�C���^���C�e���[�^���g�p�o����B
template <typename T, bool UseSentinel = false> inline void insertionSort(T first, T last) {
	if (UseSentinel) {
		assert(std::max_element(first - 1, last) == first - 1); // �ԕ����ő�l�ƂȂ邱�Ƃ��m�F
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

// �ł� score �̍��� moveStack �̃|�C���^��Ԃ��B
// MoveStack �̐��������Ƃ��Ȃ莞�Ԃ�������̂ŁA
// ��ł����܂ނƂ��Ɏg�p���Ă͂Ȃ�Ȃ��B
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

