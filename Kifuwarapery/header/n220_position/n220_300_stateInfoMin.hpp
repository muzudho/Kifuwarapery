#pragma once

#include "../n105_color___/n105_500_utilColor.hpp"
#include "../n200_score___/n200_100_utilScore.hpp"
#include "../n220_position/n220_250_changedLists.hpp"

// minimul stateinfo
struct StateInfoMin {
	Score material; // stocfish �� npMaterial �� ���A���̓_����z��Ŏ����Ă��邯�ǁA
					// ���ɕ�����K�v�͖����C������B
	int pliesFromNull;
	int continuousCheck[ColorNum]; // Stockfish �ɂ͖����B
};

