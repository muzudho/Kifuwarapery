#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_400_squareRelation.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_210_dragonAttackBb.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_500_ptArray.hpp"
#include "../n372_piece___/n372_070_PieceAbstract.hpp"


class PieceWDragon : public PieceAbstract {
public:
	void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq) const {
		if (g_squareRelation.GetSquareRelation(checkSq, ksq) & N04_DirecDiag) {
			// 斜めから王手したときは、玉の移動先と王手した駒の間に駒があることがあるので、
			// dragonAttackToEdge(checkSq) は使えない。
			bannedKingToBB |= PieceTypeArray::m_dragon.GetAttacks2From(pos.GetOccupiedBB(), Color::ColorNum, checkSq);
		}
		else {
			bannedKingToBB |= g_dragonAttackBb.GetControllBbToEdge(checkSq);
		}
	}
};
