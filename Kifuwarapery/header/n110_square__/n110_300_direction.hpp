#pragma once

#include "../n110_square__/n110_250_squareDelta.hpp"


// �ʒu�֌W�A����
// �{�i���U���̂܂�
// �ł����܂�g��Ȃ��̂ŕ��ʂ� enum �Ɠ��l�� 0 ���珇�ɒl��t���čs���Ηǂ��Ǝv���B
enum Direction {
	DirecMisc = Binary<  0>::value, // �c�A���A�΂߂̈ʒu�ɖ����ꍇ
	DirecFile = Binary< 10>::value, // �c
	DirecRank = Binary< 11>::value, // ��
	DirecDiagNESW = Binary<100>::value, // �E�ォ�獶��
	DirecDiagNWSE = Binary<101>::value, // ���ォ��E��
	DirecCross = Binary< 10>::value, // �c�A��
	DirecDiag = Binary<100>::value, // �΂�
};
OverloadEnumOperators(Direction);
