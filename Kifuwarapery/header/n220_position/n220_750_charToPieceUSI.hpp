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
#include "../n220_position/n220_400_evalList.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n230_pieceTyp/n230_110_ptPawn.hpp"
#include "../n230_pieceTyp/n230_120_ptLance.hpp"
#include "../n230_pieceTyp/n230_130_ptKnight.hpp"
#include "../n230_pieceTyp/n230_140_ptSilver.hpp"
#include "../n230_pieceTyp/n230_150_ptBishop.hpp"
#include "../n230_pieceTyp/n230_160_ptRook.hpp"
#include "../n230_pieceTyp/n230_180_ptKing.hpp"
#include "../n230_pieceTyp/n230_230_ptHorse.hpp"
#include "../n230_pieceTyp/n230_240_ptDragon.hpp"


const std::string g_DefaultStartPositionSFEN = "lnsgkgsnl/1r5b1/ppppppppp/9/9/9/PPPPPPPPP/1B5R1/LNSGKGSNL b - 1";



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

