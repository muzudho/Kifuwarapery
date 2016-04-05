#include "../../header/n160_board___/n160_105_utilBitboard.hpp"


Bitboard UtilBitboard::m_kingAttack[SquareNum];


// これらは一度値を設定したら二度と変更しない。
// 本当は const 化したい。
#if defined HAVE_BMI2
Bitboard UtilBitboard::m_rookAttack[495616];
#else
Bitboard UtilBitboard::m_rookAttack[512000];
#endif
