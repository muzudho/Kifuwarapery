#pragma once


// �w����̎��
enum MoveType {
	Capture,            // �������B     ��, ��, �p �̕s���͊܂܂Ȃ��B���̓�i�ڂ̕s�����܂܂Ȃ��B
	NonCapture,         // ������Ȃ���B ��, ��, �p �̕s���͊܂܂Ȃ��B���̓�i�ڂ̕s�����܂܂Ȃ��B
	Drop,               // ��ł��B ����A�ł����l�߂͊܂܂Ȃ��B
	CapturePlusPro,     // Capture + (�� �̋�����Ȃ������)
	NonCaptureMinusPro, // NonCapture - (�� �̋�����Ȃ������) - (���̎O�i�ڂւ̋�����Ȃ��s��)
	Recapture,          // ����̈ʒu�ւ̎��Ԃ��̎�
	Evasion,            // �������B��, ��, �p �̕s���͂͊܂܂Ȃ��B
	NonEvasion,         // ���肪�|�����Ă��Ȃ��Ƃ��̍��@�� (�ʂ̈ړ��ɂ�鎩�E��Apin����Ă����̈ړ��ɂ�鎩�E��͉�����Ȃ��B)
	Legal,              // ���肪�|�����Ă���� Evasion, �����łȂ��Ȃ� NonEvasion �𐶐����A
						// �ʂ̎��E��� pin ����Ă��̈ړ��ɂ�鎩�E���r���B(�A������̐����͔r�����Ȃ��B)
	LegalAll,           // Legal + ��, ��, �p �̕s���A���̓�i�ڂ̕s���A���̎O�i�ڂւ̋�����Ȃ��s���𐶐�
	MoveTypeNone
};
