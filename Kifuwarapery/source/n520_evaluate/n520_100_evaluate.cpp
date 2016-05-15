#include <array>
#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n113_piece___/n113_200_handPiece.hpp"
#include "../../header/n520_evaluate/n520_500_kkKkpKppStorage1.hpp"
#include "../../header/n830_evaluate/n830_700_evaluation09.hpp"

KPPBoardIndexStartToPiece g_kppBoardIndexStartToPiece;


std::array<s16, 2> KkKkpKppStorage1::KPP[SquareNum][fe_end][fe_end];
std::array<s32, 2> KkKkpKppStorage1::KKP[SquareNum][SquareNum][fe_end];
std::array<s32, 2> KkKkpKppStorage1::KK[SquareNum][SquareNum];


#if defined USE_K_FIX_OFFSET
const s32 KkKkpKppStorage1::K_Fix_Offset[SquareNum] = {
	2000*g_FVScale, 1700*g_FVScale, 1350*g_FVScale, 1000*g_FVScale,  650*g_FVScale,  350*g_FVScale,  100*g_FVScale,    0*g_FVScale,    0*g_FVScale,
	1800*g_FVScale, 1500*g_FVScale, 1250*g_FVScale, 1000*g_FVScale,  650*g_FVScale,  350*g_FVScale,  100*g_FVScale,    0*g_FVScale,    0*g_FVScale, 
	1800*g_FVScale, 1500*g_FVScale, 1250*g_FVScale, 1000*g_FVScale,  650*g_FVScale,  350*g_FVScale,  100*g_FVScale,    0*g_FVScale,    0*g_FVScale, 
	1700*g_FVScale, 1400*g_FVScale, 1150*g_FVScale,  900*g_FVScale,  550*g_FVScale,  250*g_FVScale,    0*g_FVScale,    0*g_FVScale,    0*g_FVScale, 
	1600*g_FVScale, 1300*g_FVScale, 1050*g_FVScale,  800*g_FVScale,  450*g_FVScale,  150*g_FVScale,    0*g_FVScale,    0*g_FVScale,    0*g_FVScale, 
	1700*g_FVScale, 1400*g_FVScale, 1150*g_FVScale,  900*g_FVScale,  550*g_FVScale,  250*g_FVScale,    0*g_FVScale,    0*g_FVScale,    0*g_FVScale, 
	1800*g_FVScale, 1500*g_FVScale, 1250*g_FVScale, 1000*g_FVScale,  650*g_FVScale,  350*g_FVScale,  100*g_FVScale,    0*g_FVScale,    0*g_FVScale, 
	1900*g_FVScale, 1600*g_FVScale, 1350*g_FVScale, 1000*g_FVScale,  650*g_FVScale,  350*g_FVScale,  100*g_FVScale,    0*g_FVScale,    0*g_FVScale, 
	2000*g_FVScale, 1700*g_FVScale, 1350*g_FVScale, 1000*g_FVScale,  650*g_FVScale,  350*g_FVScale,  100*g_FVScale,    0*g_FVScale,    0*g_FVScale
};
#endif

EvaluateHashTable g_evalTable;

const int kppArray[31] = {
	0,        f_pawn,   f_lance,  f_knight,
	f_silver, f_bishop, f_rook,   f_gold,   
	0,        f_gold,   f_gold,   f_gold,
	f_gold,   f_horse,  f_dragon,
	0,
	0,        e_pawn,   e_lance,  e_knight,
	e_silver, e_bishop, e_rook,   e_gold,   
	0,        e_gold,   e_gold,   e_gold,
	e_gold,   e_horse,  e_dragon
};

const int kppHandArray[g_COLOR_NUM][HandPieceNum] = {
	{f_hand_pawn, f_hand_lance, f_hand_knight, f_hand_silver,
	 f_hand_gold, f_hand_bishop, f_hand_rook},
	{e_hand_pawn, e_hand_lance, e_hand_knight, e_hand_silver,
	 e_hand_gold, e_hand_bishop, e_hand_rook}
};


