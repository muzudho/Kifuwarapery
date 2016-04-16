#include "../../header/n372_genMoveP/n372_070_pieceAbstract.hpp"
#include "../../header/n376_genMoveP/n376_080_pieceNull.hpp"
#include "../../header/n376_genMoveP/n376_190_pieceBProPawn.hpp"
#include "../../header/n376_genMoveP/n376_100_pieceEmpty.hpp"
#include "../../header/n376_genMoveP/n376_110_pieceBPawn.hpp"
#include "../../header/n376_genMoveP/n376_120_pieceBLance.hpp"
#include "../../header/n376_genMoveP/n376_130_pieceBKnight.hpp"
#include "../../header/n376_genMoveP/n376_140_pieceBSilver.hpp"
#include "../../header/n376_genMoveP/n376_150_pieceBBishop.hpp"
#include "../../header/n376_genMoveP/n376_160_pieceBRook.hpp"
#include "../../header/n376_genMoveP/n376_170_pieceBGold.hpp"
#include "../../header/n376_genMoveP/n376_180_pieceBKing.hpp"
#include "../../header/n376_genMoveP/n376_190_pieceBProPawn.hpp"
#include "../../header/n376_genMoveP/n376_200_pieceBProLance.hpp"
#include "../../header/n376_genMoveP/n376_210_pieceBProKnight.hpp"
#include "../../header/n376_genMoveP/n376_220_pieceBProSilver.hpp"
#include "../../header/n376_genMoveP/n376_230_pieceBHorse.hpp"
#include "../../header/n376_genMoveP/n376_240_pieceBDragon.hpp"
#include "../../header/n376_genMoveP/n376_270_pieceWPawn.hpp"
#include "../../header/n376_genMoveP/n376_280_pieceWLance.hpp"
#include "../../header/n376_genMoveP/n376_290_pieceWKnight.hpp"
#include "../../header/n376_genMoveP/n376_300_pieceWSilver.hpp"
#include "../../header/n376_genMoveP/n376_310_pieceWBishop.hpp"
#include "../../header/n376_genMoveP/n376_320_pieceWRook.hpp"
#include "../../header/n376_genMoveP/n376_330_pieceWGold.hpp"
#include "../../header/n376_genMoveP/n376_340_pieceWKing.hpp"
#include "../../header/n376_genMoveP/n376_350_pieceWProPawn.hpp"
#include "../../header/n376_genMoveP/n376_360_pieceWProLance.hpp"
#include "../../header/n376_genMoveP/n376_370_pieceWProKnight.hpp"
#include "../../header/n376_genMoveP/n376_380_pieceWProSilver.hpp"
#include "../../header/n376_genMoveP/n376_390_pieceWHorse.hpp"
#include "../../header/n376_genMoveP/n376_400_pieceWDragon.hpp"
#include "../../header/n376_genMoveP/n376_410_pieceNone.hpp"
#include "../../header/n376_genMoveP/n376_500_pieceArray.hpp"


PieceAbstract* g_pieceAbstractArray[32] = {
	&g_pieceEmpty,//[0]
	&g_pieceBPawn,
	&g_pieceBLance,
	&g_pieceBKnight,
	&g_pieceBSilver,
	&g_pieceBBishop,//[5]
	&g_pieceBRook,
	&g_pieceBGold,
	&g_pieceBKing,
	&g_pieceBProPawn,
	&g_pieceBProLance,//[10]
	&g_pieceBKnight,
	&g_pieceBProSilver,
	&g_pieceBHorse,
	&g_pieceBDragon,//[14]
	&g_pieceNull,//[15]
	&g_pieceNull,//[16]
	&g_pieceWPawn,
	&g_pieceWLance,
	&g_pieceWKnight,
	&g_pieceWSilver,//[20]
	&g_pieceWBishop,
	&g_pieceWRook,
	&g_pieceWGold,
	&g_pieceWKing,
	&g_pieceWProPawn,//[25]
	&g_pieceWProLance,
	&g_pieceWProKnight,
	&g_pieceWProSilver,
	&g_pieceWHorse,
	&g_pieceWDragon,//[30]
	&g_pieceNone//[31]
};

