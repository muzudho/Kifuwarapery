#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n350_pieceTyp/n350_070_ptAbstract.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n350_pieceTyp/n350_070_ptAbstract.hpp"
#include "../n350_pieceTyp/n350_100_ptOccupied.hpp"
#include "../n350_pieceTyp/n350_110_ptPawn.hpp"
#include "../n350_pieceTyp/n350_120_ptLance.hpp"
#include "../n350_pieceTyp/n350_130_ptKnight.hpp"
#include "../n350_pieceTyp/n350_140_ptSilver.hpp"
#include "../n350_pieceTyp/n350_150_ptBishop.hpp"
#include "../n350_pieceTyp/n350_160_ptRook.hpp"
#include "../n350_pieceTyp/n350_170_ptGold.hpp"
#include "../n350_pieceTyp/n350_180_ptKing.hpp"
#include "../n350_pieceTyp/n350_190_ptProPawn.hpp"
#include "../n350_pieceTyp/n350_200_ptProLance.hpp"
#include "../n350_pieceTyp/n350_210_ptProKnight.hpp"
#include "../n350_pieceTyp/n350_220_ptProSilver.hpp"
#include "../n350_pieceTyp/n350_230_ptHorse.hpp"
#include "../n350_pieceTyp/n350_240_ptDragon.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"


extern PtAbstract* g_ptArray[PieceType::N15_PieceTypeNum];
