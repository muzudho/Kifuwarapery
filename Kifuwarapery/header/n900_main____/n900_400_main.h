#pragma once

#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n320_operate_/n320_200_init.hpp"
#include "../../header/n400_usi_____/n400_250_usi.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"
#include "..\..\header\n900_main____\n900_400_main.h"


class Main {
public:
	void Initialize(std::unique_ptr<Searcher>& searcher);
	void Body(int argc, char* argv[], std::unique_ptr<Searcher>& searcher);
	void Finalize(std::unique_ptr<Searcher>& searcher);
};