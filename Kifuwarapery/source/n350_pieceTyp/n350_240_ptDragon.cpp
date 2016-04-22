﻿#include "../../header/n105_color___/n105_100_color.hpp"
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
#include "../../header/n350_pieceTyp/n350_070_ptAbstract.hpp"

#include "../../header/n350_pieceTyp/n350_240_ptDragon.hpp"
#include "../../header/n350_pieceTyp/n350_500_ptArray.hpp"


PieceType PtDragon::AppendToNextAttackerAndTryPromote(
	const Position& pos,
	const Square to,
	const Bitboard& opponentAttackers,
	Bitboard& occupied,
	Bitboard& attackers,
	const Color turn,
	PieceType nextPT
	) const {
	PieceType PT = PieceType::N14_Dragon;

	if (opponentAttackers.AndIsNot0(pos.GetBbOf10(PT))) {
		// todo: 実際に移動した方向を基にattackersを更新すれば、template, inline を使用しなくても良さそう。
		//       その場合、キャッシュに乗りやすくなるので逆に速くなるかも。
		const Bitboard bb = opponentAttackers & pos.GetBbOf10(PT);
		const Square from = bb.GetFirstOneFromI9();
		g_setMaskBb.XorBit(&occupied, from);

		attackers |= (g_lanceAttackBb.GetControllBb(occupied, UtilColor::OppositeColor(turn), to) &
			pos.GetBbOf20(N02_Lance, turn))
			| (g_lanceAttackBb.GetControllBb(occupied, turn, to) &
				pos.GetBbOf20(N02_Lance, UtilColor::OppositeColor(turn)))
			| (g_rookAttackBb.GetControllBb(occupied, to) & pos.GetBbOf20(N06_Rook, N14_Dragon))
			| (g_bishopAttackBb.BishopAttack(occupied, to) & pos.GetBbOf20(N05_Bishop, N13_Horse));

		// それ以外の駒種類は、そのまま返す☆
		return PT;
	}
	return G_NextAttacker_Recursive(
		PieceType::N14_Dragon, pos, to, opponentAttackers, occupied, attackers, turn, PieceType::N08_King);

}
