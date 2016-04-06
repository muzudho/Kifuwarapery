#include "../../header/n160_board___/n160_400_printBb.hpp"

RookAttackBb g_rookAttackBb;//本当はconst にしたいが、やり方がわからない☆ C2373エラーになるんだぜ☆

/*
//────────────────────────────────────────────────────────────────────────────────
// 飛
//────────────────────────────────────────────────────────────────────────────────
// これらは一度値を設定したら二度と変更しない。
// 本当は const 化したい。
#if defined HAVE_BMI2
Bitboard UtilBitboard::m_rookAttack[495616];
#else
Bitboard UtilBitboard::m_rookAttack[512000];
#endif

Bitboard UtilBitboard::m_rookBlockMask[SquareNum];
Bitboard UtilBitboard::m_rookAttackToEdge[SquareNum];
*/