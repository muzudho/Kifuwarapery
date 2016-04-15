#pragma once


#include <cstdlib>	// abs()
#include "../n080_common__/n080_100_common.hpp"
#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_250_squareDelta.hpp"
#include "../n161_sqDistan/n161_500_squareDistance.hpp"
#include "../n162_bonaDir_/n162_070_bonaDirAbstract.hpp"
#include "../n220_position/n220_650_position.hpp"
#include "../n350_pieceTyp/n350_150_ptBishop.hpp"


class DirecDiagNWSE : public BonaDirAbstract {
public:

	void InitializeSquareDistance(SquareDistance& squareDistance, Square sq0, Square sq1) const {
		squareDistance.SetValue( sq0, sq1,
			abs(static_cast<int>(sq0 - sq1) / static_cast<int>(SquareDelta::DeltaNW))
		);
	}

	void Do2Move(Position& position, Square from, const Square ksq, const Color us) const {
		position.GetStateInfo()->m_checkersBB |=
			g_ptBishop.GetAttacks2From(position.GetOccupiedBB(), Color::ColorNum, ksq) &
			position.GetBbOf(PieceType::N05_Bishop, PieceType::N13_Horse, us);
	}

};


extern DirecDiagNWSE g_direcDiagNWSE;

