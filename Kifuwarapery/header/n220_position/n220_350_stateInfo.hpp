#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n220_position/n220_300_stateInfoMin.hpp"


// StateInfoMin ���� memcpy �ŃR�s�[���邱�Ƃ�����̂ŁA
// �p�����g���Ă���B
struct StateInfo : public StateInfoMin {
	Key m_boardKey;
	Key m_handKey;
	// ��ԑ��̋ʂ� check ���Ă����� Bitboard
	Bitboard m_checkersBB;
	StateInfo* m_previous;
	// ��ԑ��̎�����
	Hand m_hand;
	// capturedPieceType �� move.cap() �Ŏ擾�o����̂ŕK�v�����B
	ChangedLists m_cl;

	Key GetKey() const { return m_boardKey + m_handKey; }
};
