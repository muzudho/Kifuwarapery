#pragma once


#include "../n105_color___/n105_100_color.hpp"
#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n165_movStack/n165_500_moveStack.hpp"
#include "../n220_position/n220_650_position.hpp"


class MoveTypeEvent {
public:
	MoveStack* m_moveStackList;//�ω����邺��
	const Position& m_pos;

	// ��Ԃ̐F
	const Color m_us;

	// ��Ԃ̑���̐F
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