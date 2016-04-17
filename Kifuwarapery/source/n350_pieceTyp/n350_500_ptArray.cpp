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


// デフォルト・コンストラクタは代入を省略できるぜ☆（＾ｑ＾）
const PtOccupied PieceTypeArray::m_occupied;// = PtOccupied();
const PtPawn PieceTypeArray::m_pawn;
const PtLance PieceTypeArray::m_lance;
const PtKnight PieceTypeArray::m_knight;
const PtSilver PieceTypeArray::m_silver;
const PtBishop PieceTypeArray::m_bishop;
const PtRook PieceTypeArray::m_rook;
const PtGold PieceTypeArray::m_gold;
const PtKing PieceTypeArray::m_king;
const PtProPawn PieceTypeArray::m_proPawn;
const PtProLance PieceTypeArray::m_proLance;
const PtProKnight PieceTypeArray::m_proKnight;
const PtProSilver PieceTypeArray::m_proSilver;
const PtHorse PieceTypeArray::m_horse;
const PtDragon PieceTypeArray::m_dragon;

// ポインタの配列にするのが正解☆（＾ｑ＾）
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
