#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../../header/n900_main____/n900_200_searcher.hpp"		//TODO: これを外すのがむずかしい。



class UsiLoop {
public:
	UsiLoop();

	void Mainloop(int argc, char* argv[], Searcher& searcher);
};