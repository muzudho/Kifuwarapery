#include "../../header/n160_board___/n160_105_utilBitboard.hpp"

// �����͈�x�l��ݒ肵�����x�ƕύX���Ȃ��B
// �{���� const ���������B
#if defined HAVE_BMI2
Bitboard UtilBitboard::m_rookAttack[495616];
#else
Bitboard UtilBitboard::m_rookAttack[512000];
#endif
