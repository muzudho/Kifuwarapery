#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n165_movStack/n165_600_utilMove.hpp"
#include "../n680_egOption/n680_230_engineOptionable.hpp"
#include "../n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../n885_searcher/n885_500_searcher.hpp"


class UsiLoop {
public:
	UsiLoop();

	void Mainloop(int argc, char* argv[], Searcher& searcher);
};