#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n360_egOption/n360_230_engineOptionable.hpp"
#include "../n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../n900_main____/n900_200_searcher.hpp"


class UsiLoop {
public:
	UsiLoop();

	void Mainloop(int argc, char* argv[], Searcher& searcher);
};