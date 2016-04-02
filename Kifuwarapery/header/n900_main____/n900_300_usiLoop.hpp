#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"



class UsiLoop {
public:
	UsiLoop();

	void Mainloop(int argc, char* argv[], Searcher& searcher);
};