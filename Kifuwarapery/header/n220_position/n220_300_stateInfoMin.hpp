#pragma once

#include "../../header/n120_brdEntry/n120_100_color.hpp"
#include "../../header/n220_position/n220_250_changedLists.hpp"

// minimul stateinfo
struct StateInfoMin {
	Score material; // stocfish �� npMaterial �� ���A���̓_����z��Ŏ����Ă��邯�ǁA
					// ���ɕ�����K�v�͖����C������B
	int pliesFromNull;
	int continuousCheck[ColorNum]; // Stockfish �ɂ͖����B
};

