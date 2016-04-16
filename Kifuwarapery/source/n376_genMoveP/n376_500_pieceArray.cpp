//*
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
//*/
#include "../../header/n376_genMoveP/n376_500_pieceArray.hpp"


//────────────────────────────────────────────────────────────────────────────────
// 静的メンバの実体
//────────────────────────────────────────────────────────────────────────────────
/*
const void* PieceArray::m_makeBanned2KingTo[] = {//32
	PieceEmpty::MakeBanned2KingTo,	//[0]
	PieceBPawn::MakeBanned2KingTo,
	PieceBLance::MakeBanned2KingTo,
	PieceBKnight::MakeBanned2KingTo,
	PieceBSilver::MakeBanned2KingTo,
	PieceBBishop::MakeBanned2KingTo,//[5]
	PieceBRook::MakeBanned2KingTo,
	PieceBGold::MakeBanned2KingTo,
	PieceBKing::MakeBanned2KingTo,
	PieceBProPawn::MakeBanned2KingTo,
	PieceBProLance::MakeBanned2KingTo,//[10]
	PieceBProKnight::MakeBanned2KingTo,
	PieceBProSilver::MakeBanned2KingTo,
	PieceBHorse::MakeBanned2KingTo,
	PieceBDragon::MakeBanned2KingTo,//[14]
	PieceNull::MakeBanned2KingTo,//[15]
	PieceNull::MakeBanned2KingTo,//[16]
	PieceWPawn::MakeBanned2KingTo,
	PieceWLance::MakeBanned2KingTo,
	PieceWKnight::MakeBanned2KingTo,
	PieceWSilver::MakeBanned2KingTo,//[20]
	PieceWBishop::MakeBanned2KingTo,
	PieceWRook::MakeBanned2KingTo,
	PieceWGold::MakeBanned2KingTo,
	PieceWKing::MakeBanned2KingTo,
	PieceWProPawn::MakeBanned2KingTo,//[25]
	PieceWProLance::MakeBanned2KingTo,
	PieceWProKnight::MakeBanned2KingTo,
	PieceWProSilver::MakeBanned2KingTo,
	PieceWHorse::MakeBanned2KingTo,
	PieceWDragon::MakeBanned2KingTo,//[30]
	PieceNone::MakeBanned2KingTo//[31]
};
//*/
//*
const PieceAbstract PieceArray::m_pieceAbstractArray[] = {//32
	PieceEmpty(),	//[0]
	PieceBPawn(),
	PieceBLance(),
	PieceBKnight(),
	PieceBSilver(),
	PieceBBishop(),//[5]
	PieceBRook(),
	PieceBGold(),
	PieceBKing(),
	PieceBProPawn(),
	PieceBProLance(),//[10]
	PieceBProKnight(),
	PieceBProSilver(),
	PieceBHorse(),
	PieceBDragon(),//[14]
	PieceNull(),//[15]
	PieceNull(),//[16]
	PieceWPawn(),
	PieceWLance(),
	PieceWKnight(),
	PieceWSilver(),//[20]
	PieceWBishop(),
	PieceWRook(),
	PieceWGold(),
	PieceWKing(),
	PieceWProPawn(),//[25]
	PieceWProLance(),
	PieceWProKnight(),
	PieceWProSilver(),
	PieceWHorse(),
	PieceWDragon(),//[30]
	PieceNone()//[31]
};
//*/