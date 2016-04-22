#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n110_square__/n110_500_convSquare.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n160_board___/n160_130_lanceAttackBb.hpp"
#include "../../header/n160_board___/n160_190_pawnAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n165_movStack/n165_300_moveType.hpp"
#include "../../header/n165_movStack/n165_500_moveStack.hpp"
#include "../../header/n165_movStack/n165_600_convMove.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"

#include "../../header/n350_pieceTyp/n350_040_ptEvent.hpp"
#include "../../header/n350_pieceTyp/n350_045_pieceTypeSeeEvent.hpp"
#include "../../header/n350_pieceTyp/n350_070_ptAbstract.hpp"
#include "../../header/n350_pieceTyp/n350_240_ptDragon.hpp"
#include "../../header/n350_pieceTyp/n350_500_ptPrograms.hpp"


PieceType PtDragon::AppendToNextAttackerAndTryPromote(
	Bitboard& occupied,
	Bitboard& attackers,
	PieceType nextPT,
	const PieceTypeSeeEvent ptsEvent
	) const {
	PieceType PT = PieceType::N14_Dragon;

	if (ptsEvent.m_opponentAttackers.AndIsNot0(ptsEvent.m_pos.GetBbOf10(PT))) {
		// todo: 実際に移動した方向を基にattackersを更新すれば、template, inline を使用しなくても良さそう。
		//       その場合、キャッシュに乗りやすくなるので逆に速くなるかも。
		const Bitboard bb = ptsEvent.m_opponentAttackers & ptsEvent.m_pos.GetBbOf10(PT);
		const Square from = bb.GetFirstOneFromI9();
		g_setMaskBb.XorBit(&occupied, from);

		attackers |= (g_lanceAttackBb.GetControllBb(occupied, ConvColor::OPPOSITE_COLOR10(ptsEvent.m_turn), ptsEvent.m_to) &
			ptsEvent.m_pos.GetBbOf20(N02_Lance, ptsEvent.m_turn))
			| (g_lanceAttackBb.GetControllBb(occupied, ptsEvent.m_turn, ptsEvent.m_to) &
				ptsEvent.m_pos.GetBbOf20(N02_Lance, ConvColor::OPPOSITE_COLOR10(ptsEvent.m_turn)))
			| (g_rookAttackBb.GetControllBb(occupied, ptsEvent.m_to) & ptsEvent.m_pos.GetBbOf20(N06_Rook, N14_Dragon))
			| (g_bishopAttackBb.BishopAttack(occupied, ptsEvent.m_to) & ptsEvent.m_pos.GetBbOf20(N05_Bishop, N13_Horse));

		// それ以外の駒種類は、そのまま返す☆
		return PT;
	}

	return PiecetypePrograms::m_PIECETYPE_PROGRAMS[nextPT]->AppendToNextAttackerAndTryPromote(
		occupied,
		attackers,
		PieceType::N08_King,
		ptsEvent
		);
}
