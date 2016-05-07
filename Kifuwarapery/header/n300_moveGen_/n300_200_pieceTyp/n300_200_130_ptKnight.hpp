#pragma once


#include "../../n105_color___/n105_100_color.hpp"
#include "../../n110_square__/n110_100_square.hpp"
#include "../../n110_square__/n110_500_convSquare.hpp"
#include "../../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../n160_board___/n160_100_bitboard.hpp"
#include "../../n160_board___/n160_180_knightAttackBb.hpp"
#include "../../n160_board___/n160_230_setMaskBb.hpp"
#include "../../n165_movStack/n165_300_moveType.hpp"
#include "../../n165_movStack/n165_420_convMove.hpp"
#include "../../n220_position/n220_650_position.hpp"
#include "n300_200_020_moveStack.hpp"
#include "n300_200_030_makePromoteMove.hpp""
#include "n300_200_040_ptEvent.hpp"
#include "n300_200_045_pieceTypeSeeEvent.hpp"
#include "n300_200_070_ptAbstract.hpp"


const static Move g_PTKNIGHT_ONBOARD_AS_MOVE = ConvMove::FROM_PIECETYPE_ONBOARD10(PieceType::N03_Knight);
const static Move g_PTKNIGHT_DA_AS_MOVE = ConvMove::FROM_PIECETYPE_DA10(PieceType::N03_Knight);


// ＰｉｅｃｅＴｙｐｅ：：Ｎ０３＿Ｋｎｉｇｈｔ
class PtKnight : public PtAbstract {
public:

	virtual Move AsMove() const override {
		return g_PTKNIGHT_ONBOARD_AS_MOVE;
	}

	Bitboard GetAttacks2From(const PieceTypeEvent& ptEvent) const override {
		return g_knightAttackBb.GetControllBb(ptEvent.m_c, ptEvent.m_sq);
	}

	// pin は省かない。//FORCE_INLINE
	void Generate2RecaptureMoves_usWhite(
		MoveStack* pMoveStackList,
		const Position& pos,
		const Square from,
		const Square to
		) const override {

		pMoveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(
			this->AsMove(), from, to, pos);

		if (
			ConvSquare::CAN_PROMOTE10<Color::White>(ConvSquare::TO_RANK10(to))
			|
			ConvSquare::CAN_PROMOTE10<Color::White>(ConvSquare::TO_RANK10(from))
		){
			MakePromoteMove::APPEND_PROMOTE_FLAG(pMoveStackList->m_move);
		}

		pMoveStackList++;
	}

	void Generate2RecaptureMoves_usBlack(
		MoveStack* pMoveStackList,
		const Position& pos,
		const Square from,
		const Square to
		) const override {

		pMoveStackList->m_move = g_makePromoteMove.GetSelectedMakeMove_ExceptPromote_CaptureCategory(
			this->AsMove(), from, to, pos);

		if (
			ConvSquare::CAN_PROMOTE10<Color::Black>(ConvSquare::TO_RANK10(to))
			|
			ConvSquare::CAN_PROMOTE10<Color::Black>(ConvSquare::TO_RANK10(from))
			) {
			MakePromoteMove::APPEND_PROMOTE_FLAG(pMoveStackList->m_move);
		}

		pMoveStackList++;
	}

	PieceType AppendToNextAttackerAndTryPromote(
		Bitboard& occupied,
		Bitboard& attackers,
		PieceType nextPT,
		const PieceTypeSeeEvent ptsEvent
		) const override;

};
