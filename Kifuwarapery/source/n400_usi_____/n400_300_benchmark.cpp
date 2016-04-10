#include <fstream>
#include <sstream>
#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n220_position/n220_600_position.hpp"
#include "../../header/n223_move____/n223_050_limitsType.hpp"
#include "../../header/n400_usi_____/n400_260_usiOperation.hpp"
#include "../../header/n400_usi_____/n400_300_benchmark.hpp"


#include "../../header/n900_main____/n900_200_searcher.hpp"		//TODO:1 これを外すのがむずかしい。
//class Searcher;


// 今はベンチマークというより、PGO ビルドの自動化の為にある。
void Benchmark(Position& pos) {
	std::string token;
	LimitsType limits;

	std::string options[] = {"name Threads value 1",
							 "name MultiPV value 1",
							 "name OwnBook value false",
							 "name Max_Random_Score_Diff value 0"};
	for (auto& str : options) {
		std::istringstream is(str);
		pos.GetSearcher()->SetOption(is);
	}

	std::ifstream ifs("benchmark.sfen");
	std::string sfen;
	UsiOperation usiOperation;
	while (std::getline(ifs, sfen)) {
		std::cout << sfen << std::endl;
		std::istringstream ss_sfen(sfen);

		usiOperation.SetPosition(pos, ss_sfen);

		std::istringstream ss_go("byoyomi 10000");
		usiOperation.Go(pos, ss_go);
		pos.GetSearcher()->m_threads.WaitForThinkFinished();
	}
}
