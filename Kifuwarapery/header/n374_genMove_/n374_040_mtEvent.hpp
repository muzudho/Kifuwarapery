#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n110_square__/n110_200_rank.hpp"
#include "../n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"


class MoveTypeEvent {
public:
	MoveStack* m_moveStackList;//変化するぜ☆
	const Position& m_pos;

	// 手番の色
	const Color m_us;

	// 手番の相手の色
	const Color m_oppositeColor;

	// 先手からみた７、８、９行目
	const Bitboard m_tRank789BB;
	const Bitboard m_tRank1_6BB;
	const Bitboard m_tRank1_7BB;
public:
	MoveTypeEvent(
		MoveStack* moveStackList,
		const Position& pos,
		const Color us
	) :
		m_moveStackList(moveStackList),
		m_pos(pos),
		m_us(us),
		m_oppositeColor(UtilColor::OppositeColor(us)),
		m_tRank789BB(g_inFrontMaskBb.GetInFrontMask(us, (us == Color::Black ? Rank::Rank6 : Rank::Rank4))),
		m_tRank1_6BB(g_inFrontMaskBb.GetInFrontMask(m_oppositeColor, us == Color::Black ? Rank::Rank7 : Rank::Rank3)),
		m_tRank1_7BB(g_inFrontMaskBb.GetInFrontMask(m_oppositeColor, (us == Color::Black ? Rank::Rank8 : Rank::Rank2)))
	{

	}
};