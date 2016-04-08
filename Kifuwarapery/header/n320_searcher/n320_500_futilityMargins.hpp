#pragma once


#include "../n119_score___/n119_090_score.hpp"



class FutilityMargins {
public:
	Score m_FutilityMargins[16][64]; // [depth][moveCount]

public:
	void Initialize();

};


extern FutilityMargins g_futilityMargins;
