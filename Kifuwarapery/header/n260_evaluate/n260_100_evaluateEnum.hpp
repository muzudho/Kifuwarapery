#pragma once


// �]���֐��e�[�u���̃I�t�Z�b�g�B
// f_xxx �������̋�Ae_xxx ���G�̋�
// Bonanza �̉e���Ŏ����� 0 �̏ꍇ�̃C���f�b�N�X�����݂��邪�A�Q�Ƃ��鎖�͖����B
// todo: ������ 0 �̈ʒu���l�߂ăe�[�u���������ł�����������B(�L���b�V���ɏ����͏��₷��?)
enum {
	f_hand_pawn = 0, // 0
	e_hand_pawn = f_hand_pawn + 19,
	f_hand_lance = e_hand_pawn + 19,
	e_hand_lance = f_hand_lance + 5,
	f_hand_knight = e_hand_lance + 5,
	e_hand_knight = f_hand_knight + 5,
	f_hand_silver = e_hand_knight + 5,
	e_hand_silver = f_hand_silver + 5,
	f_hand_gold = e_hand_silver + 5,
	e_hand_gold = f_hand_gold + 5,
	f_hand_bishop = e_hand_gold + 5,
	e_hand_bishop = f_hand_bishop + 3,
	f_hand_rook = e_hand_bishop + 3,
	e_hand_rook = f_hand_rook + 3,
	fe_hand_end = e_hand_rook + 3,

	f_pawn = fe_hand_end,
	e_pawn = f_pawn + 81,
	f_lance = e_pawn + 81,
	e_lance = f_lance + 81,
	f_knight = e_lance + 81,
	e_knight = f_knight + 81,
	f_silver = e_knight + 81,
	e_silver = f_silver + 81,
	f_gold = e_silver + 81,
	e_gold = f_gold + 81,
	f_bishop = e_gold + 81,
	e_bishop = f_bishop + 81,
	f_horse = e_bishop + 81,
	e_horse = f_horse + 81,
	f_rook = e_horse + 81,
	e_rook = f_rook + 81,
	f_dragon = e_rook + 81,
	e_dragon = f_dragon + 81,
	fe_end = e_dragon + 81
};


const int g_FVScale = 32;

