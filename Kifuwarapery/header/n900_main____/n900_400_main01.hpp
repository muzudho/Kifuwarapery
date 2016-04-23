#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n220_position/n220_750_charToPieceUSI.hpp"
#include "../n480_tt______/n480_300_tt.hpp"
#include "../n760_thread__/n760_400_herosPub.hpp"
#include "../n885_searcher/n885_040_rucksack.hpp"


class Main01 {
public:
	std::unique_ptr<Rucksack> searcher;

public:
	Main01();
	~Main01();
	void Initialize();
	void Body(int argc, char* argv[]);
	void Finalize();
};