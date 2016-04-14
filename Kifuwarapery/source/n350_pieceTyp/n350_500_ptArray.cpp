#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n350_pieceTyp/n350_070_ptAbstract.hpp"
#include "../../header/n350_pieceTyp/n350_100_ptOccupied.hpp"
#include "../../header/n350_pieceTyp/n350_110_ptPawn.hpp"
#include "../../header/n350_pieceTyp/n350_120_ptLance.hpp"
#include "../../header/n350_pieceTyp/n350_130_ptKnight.hpp"
#include "../../header/n350_pieceTyp/n350_140_ptSilver.hpp"
#include "../../header/n350_pieceTyp/n350_150_ptBishop.hpp"
#include "../../header/n350_pieceTyp/n350_160_ptRook.hpp"
#include "../../header/n350_pieceTyp/n350_170_ptGold.hpp"
#include "../../header/n350_pieceTyp/n350_180_ptKing.hpp"
#include "../../header/n350_pieceTyp/n350_190_ptProPawn.hpp"
#include "../../header/n350_pieceTyp/n350_200_ptProLance.hpp"
#include "../../header/n350_pieceTyp/n350_210_ptProKnight.hpp"
#include "../../header/n350_pieceTyp/n350_220_ptProSilver.hpp"
#include "../../header/n350_pieceTyp/n350_230_ptHorse.hpp"
#include "../../header/n350_pieceTyp/n350_240_ptDragon.hpp"
#include "../../header/n350_pieceTyp/n350_500_ptArray.hpp"


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
