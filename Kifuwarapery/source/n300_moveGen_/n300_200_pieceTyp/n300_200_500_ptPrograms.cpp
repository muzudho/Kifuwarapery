#include "../../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_070_ptAbstract.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_100_ptOccupied.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_110_ptPawn.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_120_ptLance.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_130_ptKnight.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_140_ptSilver.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_150_ptBishop.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_160_ptRook.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_170_ptGold.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_180_ptKing.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_190_ptProPawn.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_200_ptProLance.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_210_ptProKnight.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_220_ptProSilver.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_230_ptHorse.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_240_ptDragon.hpp"
#include "../../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_500_ptPrograms.hpp"


// デフォルト・コンストラクタは代入を省略できるぜ☆（＾ｑ＾）
const PtOccupied PiecetypePrograms::m_OCCUPIED;// = PtOccupied();
const PtPawn PiecetypePrograms::m_PAWN;
const PtLance PiecetypePrograms::m_LANCE;
const PtKnight PiecetypePrograms::m_KNIGHT;
const PtSilver PiecetypePrograms::m_SILVER;
const PtBishop PiecetypePrograms::m_BISHOP;
const PtRook PiecetypePrograms::m_ROOK;
const PtGold PiecetypePrograms::m_GOLD;
const PtKing PiecetypePrograms::m_KING;
const PtProPawn PiecetypePrograms::m_PRO_PAWN;
const PtProLance PiecetypePrograms::m_PRO_LANCE;
const PtProKnight PiecetypePrograms::m_PRO_KNIGHT;
const PtProSilver PiecetypePrograms::m_PRO_SILVER;
const PtHorse PiecetypePrograms::m_HORSE;
const PtDragon PiecetypePrograms::m_DRAGON;

// ポインタの配列にするのが正解☆（＾ｑ＾）
const PtAbstract* PiecetypePrograms::m_PIECETYPE_PROGRAMS[g_PIECETYPE_NUM] = {
	&PiecetypePrograms::m_OCCUPIED,
	&PiecetypePrograms::m_PAWN,
	&PiecetypePrograms::m_LANCE,
	&PiecetypePrograms::m_KNIGHT,
	&PiecetypePrograms::m_SILVER,
	&PiecetypePrograms::m_BISHOP,
	&PiecetypePrograms::m_ROOK,
	&PiecetypePrograms::m_GOLD,
	&PiecetypePrograms::m_KING,
	&PiecetypePrograms::m_PRO_PAWN,
	&PiecetypePrograms::m_PRO_LANCE,
	&PiecetypePrograms::m_PRO_KNIGHT,
	&PiecetypePrograms::m_PRO_SILVER,
	&PiecetypePrograms::m_HORSE,
	&PiecetypePrograms::m_DRAGON
};
