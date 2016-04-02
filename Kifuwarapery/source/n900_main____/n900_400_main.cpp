#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"
#include "../../header/n340_init____/n340_100_initializer.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"
#include "..\..\header\n900_main____/n900_300_usiLoop.hpp"
#include "..\..\header\n900_main____\n900_400_main01.hpp"


Main01::Main01()
{
	this->searcher = std::unique_ptr<Searcher>(new Searcher);
}

Main01::~Main01()
{
	this->searcher.reset();
}

void Main01::Initialize()
{
	SYNCCOUT << "(^q^)main(1/6): (long time)initTable!" << SYNCENDL;
	Initializer initializer;
	initializer.initTable();

	SYNCCOUT << "(^q^)main(2/6): initZobrist!" << SYNCENDL;
	Position::initZobrist();

	SYNCCOUT << "(^q^)main(3/6): searcher->init!" << SYNCENDL;
	this->searcher->init();
	// 一時オブジェクトの生成と破棄

	SYNCCOUT << "(^q^)main(4/6): start Evaluater init!" << SYNCENDL;
	std::unique_ptr<EvalStorage>(new EvalStorage)->init(this->searcher->options["Eval_Dir"], true);
	SYNCCOUT << "(^q^)main(5/6): end Evaluater init! ----> doUSICommandLoop" << SYNCENDL;

}

void Main01::Body(int argc, char* argv[])
{
	UsiLoop usiLoop;
	usiLoop.Mainloop(argc, argv, *this->searcher);
}

void Main01::Finalize()
{
	SYNCCOUT << "(^q^)main(6/6): threads.exit! ----> doUSICommandLoop" << SYNCENDL;
	this->searcher->threads.exit();
}
