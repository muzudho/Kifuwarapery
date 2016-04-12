#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n170_pieceTyp/n170_070_ptAbstract.hpp"
#include "../../header/n170_pieceTyp/n170_100_ptOccupied.hpp"
#include "../../header/n170_pieceTyp/n170_110_ptPawn.hpp"
#include "../../header/n170_pieceTyp/n170_120_ptLance.hpp"
#include "../../header/n170_pieceTyp/n170_130_ptKnight.hpp"
#include "../../header/n170_pieceTyp/n170_140_ptSilver.hpp"
#include "../../header/n170_pieceTyp/n170_150_ptBishop.hpp"
#include "../../header/n170_pieceTyp/n170_160_ptRook.hpp"
#include "../../header/n170_pieceTyp/n170_170_ptGold.hpp"
#include "../../header/n170_pieceTyp/n170_180_ptKing.hpp"
#include "../../header/n170_pieceTyp/n170_190_ptProPawn.hpp"
#include "../../header/n170_pieceTyp/n170_200_ptProLance.hpp"
#include "../../header/n170_pieceTyp/n170_210_ptProKnight.hpp"
#include "../../header/n170_pieceTyp/n170_220_ptProSilver.hpp"
#include "../../header/n170_pieceTyp/n170_230_ptHorse.hpp"
#include "../../header/n170_pieceTyp/n170_240_ptDragon.hpp"
#include "../../header/n170_pieceTyp/n170_500_ptArray.hpp"


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

