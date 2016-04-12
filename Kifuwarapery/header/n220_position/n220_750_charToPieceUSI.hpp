#pragma once

#include <stack>
#include <memory>
#include <map>
#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n113_piece___/n113_500_hand.hpp"
#include "../n119_score___/n119_200_pieceScore.hpp"
#include "../n160_board___/n160_110_silverAttackBb.hpp"
#include "../n160_board___/n160_120_bishopAttackBb.hpp"
#include "../n160_board___/n160_130_lanceAttackBb.hpp"
#include "../n160_board___/n160_150_rookAttackBb.hpp"
#include "../n160_board___/n160_160_kingAttackBb.hpp"
#include "../n160_board___/n160_180_knightAttackBb.hpp"
#include "../n160_board___/n160_190_pawnAttackBb.hpp"
#include "../n160_board___/n160_200_horseAttackBb.hpp"
#include "../n160_board___/n160_210_dragonAttackBb.hpp"
#include "../n160_board___/n160_400_printBb.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n170_pieceTyp/n170_110_ptPawn.hpp"
#include "../n170_pieceTyp/n170_120_ptLance.hpp"
#include "../n170_pieceTyp/n170_130_ptKnight.hpp"
#include "../n170_pieceTyp/n170_140_ptSilver.hpp"
#include "../n170_pieceTyp/n170_150_ptBishop.hpp"
#include "../n170_pieceTyp/n170_160_ptRook.hpp"
#include "../n170_pieceTyp/n170_180_ptKing.hpp"
#include "../n170_pieceTyp/n170_230_ptHorse.hpp"
#include "../n170_pieceTyp/n170_240_ptDragon.hpp"
#include "../n220_position/n220_400_evalList.hpp"
#include "../n220_position/n220_650_position.hpp"


const std::string g_DefaultStartPositionSFEN = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";


/*
template <> inline Bitboard Position::GetAttacksFrom<N01_Pawn  >(const Color c, const Square sq, const Bitboard& nullOccupied) const { return g_ptPawn.GetAttacks2From(nullOccupied, c, sq); }
template <> inline Bitboard Position::GetAttacksFrom<N02_Lance >(const Color c, const Square sq, const Bitboard& occupied) const { return g_ptLance.GetAttacks2From(occupied, c, sq); }//this->GetOccupiedBB()
template <> inline Bitboard Position::GetAttacksFrom<N03_Knight>(const Color c, const Square sq, const Bitboard& nullOccupied) const { return g_ptKnight.GetAttacks2From(g_nullBitboard, c, sq); }
template <> inline Bitboard Position::GetAttacksFrom<N04_Silver>(const Color c, const Square sq, const Bitboard& nullOccupied) const { return g_ptSilver.GetAttacks2From(g_nullBitboard, c, sq); }
template <> inline Bitboard Position::GetAttacksFrom<N05_Bishop>(const Color, const Square sq, const Bitboard& occupied) const { return g_ptBishop.GetAttacks2From(occupied, Color::ColorNum, sq); }//Colorは使ってない。//this->GetOccupiedBB()
template <> inline Bitboard Position::GetAttacksFrom<N06_Rook  >(const Color, const Square sq, const Bitboard& occupied) const { return g_ptRook.GetAttacks2From(occupied, Color::ColorNum, sq); }//Colorは使ってない。//this->GetOccupiedBB()
template <> inline Bitboard Position::GetAttacksFrom<N07_Gold  >(const Color c, const Square sq, const Bitboard& nullOccupied)const { return g_ptGold.GetAttacks2From(g_nullBitboard, c, sq); }// 金のみ、occupiedビットボードは不要☆（＾ｑ＾）
template <> inline Bitboard Position::GetAttacksFrom<N08_King  >(const Color, const Square sq, const Bitboard& nullOccupied) const { return g_ptKing.GetAttacks2From(g_nullBitboard, Color::ColorNum, sq); }
template <> inline Bitboard Position::GetAttacksFrom<N13_Horse >(const Color, const Square sq, const Bitboard& occupied) const { return g_ptHorse.GetAttacks2From(occupied, Color::ColorNum, sq); }//this->GetOccupiedBB()
template <> inline Bitboard Position::GetAttacksFrom<N14_Dragon>(const Color, const Square sq, const Bitboard& occupied) const { return g_ptDragon.GetAttacks2From(occupied, Color::ColorNum, sq); }//this->GetOccupiedBB()
*/
/*																																																// 角飛馬竜は Color は何でもいいので ColorNum を入れている。
template <PieceType PT>
static Bitboard GetAttacksFrom(const Color c, const Square sq, const Bitboard& occupied);

template <PieceType PT>
Bitboard GetAttacksFrom(const Square sq) const {
	static_assert(PT == N05_Bishop || PT == N06_Rook || PT == N08_King || PT == N13_Horse || PT == N14_Dragon, "角飛玉馬竜しか、これを使ってはいけない？（＾ｑ＾）");
	// Color は何でも良い。
	return GetAttacksFrom<PT>(ColorNum, sq);
}
*/


// position sfen R8/2K1S1SSk/4B4/9/9/9/9/9/1L1L1L3 b PLNSGBR17p3n3g 1
// の局面が最大合法手局面で 593 手。番兵の分、+ 1 しておく。
const int g_MaxLegalMoves = 593 + 1;

class CharToPieceUSI : public std::map<char, Piece> {
public:

	CharToPieceUSI() {
		(*this)['P'] = BPawn;   (*this)['p'] = WPawn;
		(*this)['L'] = BLance;  (*this)['l'] = WLance;
		(*this)['N'] = BKnight; (*this)['n'] = WKnight;
		(*this)['S'] = BSilver; (*this)['s'] = WSilver;
		(*this)['B'] = BBishop; (*this)['b'] = WBishop;
		(*this)['R'] = BRook;   (*this)['r'] = WRook;
		(*this)['G'] = BGold;   (*this)['g'] = WGold;
		(*this)['K'] = BKing;   (*this)['k'] = WKing;
	}

	Piece GetValue(char c) const      { return this->find(c)->second; }

	bool IsLegalChar(char c) const { return (this->find(c) != this->end()); }

};


extern const CharToPieceUSI g_charToPieceUSI;

