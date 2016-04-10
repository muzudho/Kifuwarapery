#pragma once


#include <unordered_map>
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n160_board___/n160_600_bitboardAll.hpp"
#include "../n260_evaluate/n260_150_kppIndexArray.hpp"


struct KPPBoardIndexStartToPiece : public std::unordered_map<int, Piece> {
	KPPBoardIndexStartToPiece() {
		(*this)[f_pawn] = BPawn;
		(*this)[e_pawn] = WPawn;
		(*this)[f_lance] = BLance;
		(*this)[e_lance] = WLance;
		(*this)[f_knight] = BKnight;
		(*this)[e_knight] = WKnight;
		(*this)[f_silver] = BSilver;
		(*this)[e_silver] = WSilver;
		(*this)[f_gold] = BGold;
		(*this)[e_gold] = WGold;
		(*this)[f_bishop] = BBishop;
		(*this)[e_bishop] = WBishop;
		(*this)[f_horse] = BHorse;
		(*this)[e_horse] = WHorse;
		(*this)[f_rook] = BRook;
		(*this)[e_rook] = WRook;
		(*this)[f_dragon] = BDragon;
		(*this)[e_dragon] = WDragon;
	}
	Piece value(const int i) const {
		const auto it = find(i);
		if (it == std::end(*this))
			return PieceNone;
		return it->second;
	}
};
extern KPPBoardIndexStartToPiece g_kppBoardIndexStartToPiece;

