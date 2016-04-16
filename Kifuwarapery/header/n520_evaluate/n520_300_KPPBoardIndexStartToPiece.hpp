#pragma once


#include <unordered_map>
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n520_evaluate/n520_150_kppIndexArray.hpp"


struct KPPBoardIndexStartToPiece : public std::unordered_map<int, Piece> {
	KPPBoardIndexStartToPiece() {
		(*this)[f_pawn] = N01_BPawn;
		(*this)[e_pawn] = N17_WPawn;
		(*this)[f_lance] = N02_BLance;
		(*this)[e_lance] = N18_WLance;
		(*this)[f_knight] = N03_BKnight;
		(*this)[e_knight] = N19_WKnight;
		(*this)[f_silver] = N04_BSilver;
		(*this)[e_silver] = N20_WSilver;
		(*this)[f_gold] = N07_BGold;
		(*this)[e_gold] = N23_WGold;
		(*this)[f_bishop] = N05_BBishop;
		(*this)[e_bishop] = N21_WBishop;
		(*this)[f_horse] = N13_BHorse;
		(*this)[e_horse] = N29_WHorse;
		(*this)[f_rook] = N06_BRook;
		(*this)[e_rook] = N22_WRook;
		(*this)[f_dragon] = N14_BDragon;
		(*this)[e_dragon] = N30_WDragon;
	}
	Piece value(const int i) const {
		const auto it = find(i);
		if (it == std::end(*this))
			return N31_PieceNone;
		return it->second;
	}
};
extern KPPBoardIndexStartToPiece g_kppBoardIndexStartToPiece;

