#include <cstdlib>	// abs()
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n110_square__/n110_250_squareDelta.hpp"
#include "../../header/n161_sqDistan/n161_500_squareDistance.hpp"
#include "../../header/n162_bonaDir_/n162_070_bonaDirAbstract.hpp"
#include "../../header/n220_position/n220_650_position.hpp" // FIXME:
#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_040_ptEvent.hpp"
#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_500_ptPrograms.hpp"
#include "../../header/n351_bonaDir_/n351_150_direcDiagNWSE.hpp"


DirecDiagNWSE g_direcDiagNWSE;


void DirecDiagNWSE::Do2Move(Position& position, Square from, const Square ksq, const Color us) const {
	const PieceTypeEvent ptEvent1(position.GetOccupiedBB(), Color::Null, ksq);
	position.GetStateInfo()->m_checkersBB |=
		PiecetypePrograms::m_BISHOP.GetAttacks2From(ptEvent1) &
		(
			us == Color::Black
			?
			position.GetBbOf30<Color::Black>(PieceType::N05_Bishop, PieceType::N13_Horse)
			:
			position.GetBbOf30<Color::White>(PieceType::N05_Bishop, PieceType::N13_Horse)
			)
		;
}
