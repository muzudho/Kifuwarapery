#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n320_operate_/n320_200_init.hpp"
#include "../../header/n400_usi_____/n400_250_usi.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"
#include "..\..\header\n900_main____\n900_400_main.h"


void Main::Initialize(std::unique_ptr<Searcher>& searcher)
{
	SYNCCOUT << "(^q^)main(1/6): (long time)initTable!" << SYNCENDL;
	Initializer initializer;
	initializer.initTable();

	SYNCCOUT << "(^q^)main(2/6): initZobrist!" << SYNCENDL;
	Position::initZobrist();

	SYNCCOUT << "(^q^)main(3/6): searcher->init!" << SYNCENDL;
	searcher->init();
	// 一時オブジェクトの生成と破棄

	SYNCCOUT << "(^q^)main(4/6): start Evaluater init!" << SYNCENDL;
	std::unique_ptr<Evaluater>(new Evaluater)->init(searcher->options["Eval_Dir"], true);
	SYNCCOUT << "(^q^)main(5/6): end Evaluater init! ----> doUSICommandLoop" << SYNCENDL;

}

void Main::Body(int argc, char* argv[], std::unique_ptr<Searcher>& searcher)
{
	searcher->doUSICommandLoop(argc, argv);
}

void Main::Finalize(std::unique_ptr<Searcher>& searcher)
{
	SYNCCOUT << "(^q^)main(6/6): threads.exit! ----> doUSICommandLoop" << SYNCENDL;
	searcher->threads.exit();
}
