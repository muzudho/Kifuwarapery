#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n400_usi_____/n400_260_usiOperation.hpp"
#include "../../header/n400_usi_____/n400_300_benchmark.hpp"

// 今はベンチマークというより、PGO ビルドの自動化の為にある。
void benchmark(Position& pos) {
	std::string token;
	LimitsType limits;

	std::string options[] = {"name Threads value 1",
							 "name MultiPV value 1",
							 "name OwnBook value false",
							 "name Max_Random_Score_Diff value 0"};
	for (auto& str : options) {
		std::istringstream is(str);
		pos.searcher()->setOption(is);
	}

	std::ifstream ifs("benchmark.sfen");
	std::string sfen;
	UsiOperation usiOperation;
	while (std::getline(ifs, sfen)) {
		std::cout << sfen << std::endl;
		std::istringstream ss_sfen(sfen);

		usiOperation.setPosition(pos, ss_sfen);

		std::istringstream ss_go("byoyomi 10000");
		usiOperation.go(pos, ss_go);
		pos.searcher()->threads.waitForThinkFinished();
	}
}
