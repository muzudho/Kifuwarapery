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


const PtOccupied PieceTypeArray::m_occupied = PtOccupied();
const PtPawn PieceTypeArray::m_pawn = PtPawn();
const PtLance PieceTypeArray::m_lance = PtLance();
const PtKnight PieceTypeArray::m_knight = PtKnight();
const PtSilver PieceTypeArray::m_silver = PtSilver();
const PtBishop PieceTypeArray::m_bishop = PtBishop();
const PtRook PieceTypeArray::m_rook = PtRook();
const PtGold PieceTypeArray::m_gold = PtGold();
const PtKing PieceTypeArray::m_king = PtKing();
const PtProPawn PieceTypeArray::m_proPawn = PtProPawn();
const PtProLance PieceTypeArray::m_proLance = PtProLance();
const PtProKnight PieceTypeArray::m_proKnight = PtProKnight();
const PtProSilver PieceTypeArray::m_proSilver = PtProSilver();
const PtHorse PieceTypeArray::m_horse = PtHorse();
const PtDragon PieceTypeArray::m_dragon = PtDragon();

const PtAbstract* PieceTypeArray::m_ptArray[PieceType::N15_PieceTypeNum] = {
	&PieceTypeArray::m_occupied,
	&PieceTypeArray::m_pawn,
	&PieceTypeArray::m_lance,
	&PieceTypeArray::m_knight,
	&PieceTypeArray::m_silver,
	&PieceTypeArray::m_bishop,
	&PieceTypeArray::m_rook,
	&PieceTypeArray::m_gold,
	&PieceTypeArray::m_king,
	&PieceTypeArray::m_proPawn,
	&PieceTypeArray::m_proLance,
	&PieceTypeArray::m_proKnight,
	&PieceTypeArray::m_proSilver,
	&PieceTypeArray::m_horse,
	&PieceTypeArray::m_dragon
};
