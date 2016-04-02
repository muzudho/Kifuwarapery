
#include "../../header/n110_square__/n110_500_utilSquare.hpp"

Direction g_squareRelation[SquareNum][SquareNum];

// 何かの駒で一手で行ける位置関係についての距離のテーブル。桂馬の位置は距離1とする。
int g_squareDistance[SquareNum][SquareNum];
