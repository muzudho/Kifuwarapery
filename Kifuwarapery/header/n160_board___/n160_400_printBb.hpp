#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_110_kingAttack.hpp"
#include "n160_120_silverAttack.hpp"
#include "n160_130_bishopAttack.hpp"
#include "n160_140_lanceAttack.hpp"
#include "n160_150_goldAttack.hpp"
#include "n160_160_rookAttack.hpp"
#include "n160_170_goldAndSilverAttack.hpp"
#include "n160_180_knightAttack.hpp"
#include "n160_190_pawnAttack.hpp"
#include "n160_200_horseAttackBb.hpp"
#include "n160_210_dragonAttackBb.hpp"
#include "n160_220_queenAttackBb.hpp"
#include "n160_230_setMaskBb.hpp"
#include "n160_240_betweenBb.hpp"

extern KingAttackBb g_kingAttackBb;
extern SilverAttackBb g_silverAttackBb;
extern BishopAttackBb g_bishopAttackBb;
extern LanceAttackBb g_lanceAttackBb;
extern GoldAttackBb g_goldAttackBb;
extern RookAttackBb g_rookAttackBb;
extern GoldAndSilverAttackBb g_goldAndSilverAttackBb;
extern KnightAttackBb g_knightAttackBb;
extern PawnAttackBb g_pawnAttackBb;
extern HorseAttackBb g_horseAttackBb;
extern DragonAttackBb g_dragonAttackBb;
extern QueenAttackBb g_queenAttackBb;
extern SetMaskBb g_setMaskBb;
extern BetweenBb g_betweenBb;


class PrintBb {
public:

	//„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ
	// ‚»‚Ì‘¼
	//„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ„Ÿ

	// for debug
	static void PrintBoard(const Bitboard* thisBitboard);//const

};

