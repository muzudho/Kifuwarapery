#pragma once

#include <stack>
#include <memory>
#include <map>
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n113_piece___/n113_205_utilHandPiece.hpp"
#include "../../header/n116_hand____/n116_500_hand.hpp"
#include "../../header/n160_board___/n160_400_bitboardAll.hpp"
#include "../../header/n200_score___/n200_200_pieceScore.hpp"
#include "../../header/n220_position/n220_400_position.hpp"
#include "../../header/n220_position/n220_600_evalList.hpp"


const std::string g_DefaultStartPositionSFEN = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";



template <> inline Bitboard Position::GetAttacksFrom<Lance >(const Color c, const Square sq, const Bitboard& occupied) { return  occupied.LanceAttack(c, sq); }
template <> inline Bitboard Position::GetAttacksFrom<Bishop>(const Color  , const Square sq, const Bitboard& occupied) { return occupied.BishopAttack(   sq); }
template <> inline Bitboard Position::GetAttacksFrom<Rook  >(const Color  , const Square sq, const Bitboard& occupied) { return   occupied.RookAttack(   sq); }
template <> inline Bitboard Position::GetAttacksFrom<Horse >(const Color  , const Square sq, const Bitboard& occupied) { return  occupied.HorseAttack(   sq); }
template <> inline Bitboard Position::GetAttacksFrom<Dragon>(const Color  , const Square sq, const Bitboard& occupied) { return occupied.DragonAttack(   sq); }

template <> inline Bitboard Position::GetAttacksFrom<Pawn  >(const Color c, const Square sq) const { return   Bitboard::PawnAttack(c, sq              ); }
template <> inline Bitboard Position::GetAttacksFrom<Lance >(const Color c, const Square sq) const { return  GetOccupiedBB().LanceAttack(c, sq); }
template <> inline Bitboard Position::GetAttacksFrom<Knight>(const Color c, const Square sq) const { return Bitboard::KnightAttack(c, sq              ); }
template <> inline Bitboard Position::GetAttacksFrom<Silver>(const Color c, const Square sq) const { return Bitboard::SilverAttack(c, sq              ); }
template <> inline Bitboard Position::GetAttacksFrom<Bishop>(const Color  , const Square sq) const { return GetOccupiedBB().BishopAttack(   sq); }
template <> inline Bitboard Position::GetAttacksFrom<Rook  >(const Color  , const Square sq) const { return   GetOccupiedBB().RookAttack(   sq); }
template <> inline Bitboard Position::GetAttacksFrom<King  >(const Color  , const Square sq) const { return   Bitboard::KingAttack(   sq              ); }
template <> inline Bitboard Position::GetAttacksFrom<Horse >(const Color  , const Square sq) const { return  GetOccupiedBB().HorseAttack(   sq); }
template <> inline Bitboard Position::GetAttacksFrom<Dragon>(const Color  , const Square sq) const { return GetOccupiedBB().DragonAttack(   sq); }

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
	Piece value(char c) const      { return this->find(c)->second; }
	bool isLegalChar(char c) const { return (this->find(c) != this->end()); }
};
extern const CharToPieceUSI g_charToPieceUSI;
