#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n170_pieceTyp/n170_070_ptAbstract.hpp"
#include "../n170_pieceTyp/n170_080_ptPromote.hpp"
#include "../n170_pieceTyp/n170_100_ptOccupied.hpp"
#include "../n170_pieceTyp/n170_110_ptPawn.hpp"
#include "../n170_pieceTyp/n170_120_ptLance.hpp"
#include "../n170_pieceTyp/n170_130_ptKnight.hpp"
#include "../n170_pieceTyp/n170_140_ptSilver.hpp"
#include "../n170_pieceTyp/n170_150_ptBishop.hpp"
#include "../n170_pieceTyp/n170_160_ptRook.hpp"
#include "../n170_pieceTyp/n170_170_ptGold.hpp"
#include "../n170_pieceTyp/n170_180_ptKing.hpp"
#include "../n170_pieceTyp/n170_190_ptProPawn.hpp"
#include "../n170_pieceTyp/n170_200_ptProLance.hpp"
#include "../n170_pieceTyp/n170_210_ptProKnight.hpp"
#include "../n170_pieceTyp/n170_220_ptProSilver.hpp"
#include "../n170_pieceTyp/n170_230_ptHorse.hpp"
#include "../n170_pieceTyp/n170_240_ptDragon.hpp"
#include "../n170_pieceTyp/n170_250_ptPieceTypeNum.hpp"
#include "../n170_pieceTyp/n170_260_ptGoldHorseDragon.hpp"


PtAbstract* g_ptArray[PieceType::N15_PieceTypeNum] = {
	&g_ptOccupied,
	&g_ptPawn,
	&g_ptLance,
	&g_ptKnight,
	&g_ptSilver,
	&g_ptBishop,
	&g_ptRook,
	&g_ptGold,
	&g_ptKing,
	&g_ptProPawn,
	&g_ptProLance,
	&g_ptProKnight,
	&g_ptProSilver,
	&g_ptHorse,
	&g_ptDragon
};

