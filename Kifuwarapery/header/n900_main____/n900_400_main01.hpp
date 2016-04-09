#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n220_position/n220_500_charToPieceUSI.hpp"
#include "../n240_tt______/n240_300_tt.hpp"
#include "../n320_searcher/n320_550_searcher.hpp"
#include "../n450_thread__/n450_400_threadPool.hpp"


class Main01 {
public:
	std::unique_ptr<Searcher> searcher;

public:
	Main01();
	~Main01();
	void Initialize();
	void Body(int argc, char* argv[]);
	void Finalize();
};