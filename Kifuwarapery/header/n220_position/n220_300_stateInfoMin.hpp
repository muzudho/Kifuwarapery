#pragma once

#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n200_score___/n200_100_utilScore.hpp"
#include "../n220_position/n220_250_changedLists.hpp"

// minimul stateinfo
struct StateInfoMin {
	Score m_material; // stocfish �� npMaterial �� ���A���̓_����z��Ŏ����Ă��邯�ǁA
					// ���ɕ�����K�v�͖����C������B
	int m_pliesFromNull;
	int m_continuousCheck[ColorNum]; // Stockfish �ɂ͖����B
};

