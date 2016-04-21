#pragma once

#include <stack>
#include <memory>
#include <map>
#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_205_convHandPiece.hpp"
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
#include "../n220_position/n220_400_evalList.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_110_ptPawn.hpp"
#include "../n350_pieceTyp/n350_120_ptLance.hpp"
#include "../n350_pieceTyp/n350_130_ptKnight.hpp"
#include "../n350_pieceTyp/n350_140_ptSilver.hpp"
#include "../n350_pieceTyp/n350_150_ptBishop.hpp"
#include "../n350_pieceTyp/n350_160_ptRook.hpp"
#include "../n350_pieceTyp/n350_180_ptKing.hpp"
#include "../n350_pieceTyp/n350_230_ptHorse.hpp"
#include "../n350_pieceTyp/n350_240_ptDragon.hpp"


const std::string g_DefaultStartPositionSFEN = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";



class CharToPieceUSI : public std::map<char, Piece> {
public:

	CharToPieceUSI() {
		(*this)['P'] = N01_BPawn;   (*this)['p'] = N17_WPawn;
		(*this)['L'] = N02_BLance;  (*this)['l'] = N18_WLance;
		(*this)['N'] = N03_BKnight; (*this)['n'] = N19_WKnight;
		(*this)['S'] = N04_BSilver; (*this)['s'] = N20_WSilver;
		(*this)['B'] = N05_BBishop; (*this)['b'] = N21_WBishop;
		(*this)['R'] = N06_BRook;   (*this)['r'] = N22_WRook;
		(*this)['G'] = N07_BGold;   (*this)['g'] = N23_WGold;
		(*this)['K'] = N08_BKing;   (*this)['k'] = N24_WKing;
	}

	Piece GetValue(char c) const      { return this->find(c)->second; }

	bool IsLegalChar(char c) const { return (this->find(c) != this->end()); }

};


extern const CharToPieceUSI g_charToPieceUSI;

