#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"


class MoveTypeEvent {
public:
	MoveStack* m_moveStackList;//•Ï‰»‚·‚é‚º™
	const Position& m_pos;

	// è”Ô‚ÌF
	const Color m_us;

	// è”Ô‚Ì‘Šè‚ÌF
	const Color m_oppositeColor;

public:
	MoveTypeEvent(
		MoveStack* moveStackList,
		const Position& pos,
		const Color us
	) :
		m_moveStackList(moveStackList),
		m_pos(pos),
		m_us(us),
		m_oppositeColor(UtilColor::OppositeColor(us))
	{

	}
};