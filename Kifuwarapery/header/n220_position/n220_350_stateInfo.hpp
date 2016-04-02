#pragma once

#include "../../header/n220_position/n220_300_stateInfoMin.hpp"


// StateInfoMin ���� memcpy �ŃR�s�[���邱�Ƃ�����̂ŁA
// �p�����g���Ă���B
struct StateInfo : public StateInfoMin {
	Key boardKey;
	Key handKey;
	// ��ԑ��̋ʂ� check ���Ă����� Bitboard
	Bitboard checkersBB;
	StateInfo* previous;
	// ��ԑ��̎�����
	Hand hand;
	// capturedPieceType �� move.cap() �Ŏ擾�o����̂ŕK�v�����B
	ChangedLists cl;

	Key key() const { return boardKey + handKey; }
};
