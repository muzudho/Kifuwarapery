#pragma once

#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n230_pieceTyp/n230_070_ptAbstract.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n230_pieceTyp/n230_070_ptAbstract.hpp"
#include "../../header/n230_pieceTyp/n230_100_ptOccupied.hpp"
#include "../../header/n230_pieceTyp/n230_110_ptPawn.hpp"
#include "../../header/n230_pieceTyp/n230_120_ptLance.hpp"
#include "../../header/n230_pieceTyp/n230_130_ptKnight.hpp"
#include "../../header/n230_pieceTyp/n230_140_ptSilver.hpp"
#include "../../header/n230_pieceTyp/n230_150_ptBishop.hpp"
#include "../../header/n230_pieceTyp/n230_160_ptRook.hpp"
#include "../../header/n230_pieceTyp/n230_170_ptGold.hpp"
#include "../../header/n230_pieceTyp/n230_180_ptKing.hpp"
#include "../../header/n230_pieceTyp/n230_190_ptProPawn.hpp"
#include "../../header/n230_pieceTyp/n230_200_ptProLance.hpp"
#include "../../header/n230_pieceTyp/n230_210_ptProKnight.hpp"
#include "../../header/n230_pieceTyp/n230_220_ptProSilver.hpp"
#include "../../header/n230_pieceTyp/n230_230_ptHorse.hpp"
#include "../../header/n230_pieceTyp/n230_240_ptDragon.hpp"
#include "../../header/n230_pieceTyp/n230_500_ptArray.hpp"


extern PtAbstract* g_ptArray[PieceType::N15_PieceTypeNum];
