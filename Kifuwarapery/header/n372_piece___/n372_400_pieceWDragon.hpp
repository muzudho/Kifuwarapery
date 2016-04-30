#pragma once


#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_400_squareRelation.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_210_dragonAttackBb.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_040_ptEvent.hpp"
#include "../n350_pieceTyp/n350_500_ptPrograms.hpp"
#include "../n372_piece___/n372_070_PieceAbstract.hpp"


class PieceWDragon : public PieceAbstract {
public:
	void MakeBanned2KingTo(Bitboard& bannedKingToBB, const Position& pos, const Square checkSq, const Square ksq
		) const override {
		if (g_squareRelation.GetSquareRelation(checkSq, ksq) & N04_DirecDiag) {
			// 斜めから王手したときは、玉の移動先と王手した駒の間に駒があることがあるので、
			// dragonAttackToEdge(checkSq) は使えない。
			const PieceTypeEvent ptEvent1(pos.GetOccupiedBB(), Color::Null, checkSq);
			bannedKingToBB |= PiecetypePrograms::m_DRAGON.GetAttacks2From(ptEvent1);
		}
		else {
			bannedKingToBB |= g_dragonAttackBb.GetControllBbToEdge(checkSq);
		}
	}
};
