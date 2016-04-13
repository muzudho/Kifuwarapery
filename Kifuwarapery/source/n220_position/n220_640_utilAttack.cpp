#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n220_position/n220_640_utilAttack.hpp"
#include "../../header/n230_pieceTyp/n230_070_ptAbstract.hpp"
#include "../../header/n230_pieceTyp/n230_500_ptArray.hpp"


// 定義☆（＾ｑ＾）
//UtilAttack g_utilAttack;


Bitboard UtilAttack::GetAttacksFrom(const PieceType pType, const Color c, const Square sq, const Bitboard& occupied) {//const
	// FIXME: 配列の範囲チェックをしてないぜ☆（＾ｑ＾）
	return g_ptArray[pType]->GetAttacks2From(occupied, c, sq);
}
