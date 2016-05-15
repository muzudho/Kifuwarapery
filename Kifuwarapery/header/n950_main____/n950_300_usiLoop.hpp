#pragma once

//#include "../n080_common__/n080_100_common.hpp"
//#include "../n165_movStack/n165_420_convMove.hpp"
//#include "../n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"
//#include "../n680_egOption/n680_230_engineOptionable.hpp"


class UsiLoop {
public:
	UsiLoop();

	void Mainloop(int argc, char* argv[], Rucksack& searcher);
};