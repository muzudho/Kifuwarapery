#pragma once

#include "n160_100_bitboard.hpp"
#include "n160_110_silverAttackBb.hpp"
#include "n160_120_bishopAttackBb.hpp"
#include "n160_130_lanceAttackBb.hpp"
#include "n160_140_goldAttackBb.hpp"
#include "n160_150_rookAttackBb.hpp"
#include "n160_160_kingAttackBb.hpp"
#include "n160_170_goldAndSilverAttackBb.hpp"
#include "n160_180_knightAttackBb.hpp"
#include "n160_190_pawnAttackBb.hpp"
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

