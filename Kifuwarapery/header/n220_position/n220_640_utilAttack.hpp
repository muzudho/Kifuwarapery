#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"


class UtilAttack {
public:
	template<const Color US>
	static inline Bitboard GetAttacksFrom(const PieceType pType, const Square sq, const Bitboard& occupied) {//const																														  
		const PieceTypeEvent ptEvent1(occupied, US, sq);
		return PiecetypePrograms::m_PIECETYPE_PROGRAMS[pType]->GetAttacks2From(ptEvent1);// FIXME: 配列の範囲チェックをしてないぜ☆（＾ｑ＾）
	}
	// TODO: inline にするには、positon と utilAttackの依存関係の解消が必要☆？（＾ｑ＾）
	//static
	//inline
	static Bitboard GetAttacksFrom(const PieceType pt, const Color c, const Square sq, const Bitboard& occupied);//const

};


//extern UtilAttack g_utilAttack;
