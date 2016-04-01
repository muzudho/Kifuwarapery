#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n280_move____/n280_100_generateMoves.hpp"
#include "../../header/n280_move____/n280_150_movePicker.hpp"
#include "../../header/n320_operate_/n320_100_book.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../../header/n360_egOption/n360_245_engineOption.hpp"
#include "../../header/n400_usi_____/n400_260_usiOperation.hpp"
#include "../../header/n400_usi_____/n400_300_benchmark.hpp"
#include "../../header/n400_usi_____/n400_400_learner.hpp"
#include "..\..\header\n900_main____\n900_300_usiLoop.hpp"

#if !defined MINIMUL
// for debug
// 指し手生成の速度を計測
void measureGenerateMoves(const Position& pos) {
	pos.print();

	MoveStack legalMoves[MaxLegalMoves];
	for (int i = 0; i < MaxLegalMoves; ++i)
	{
		legalMoves[i].move = moveNone();
	}
	MoveStack* pms = &legalMoves[0];
	const u64 num = 5000000;
	Time t = Time::currentTime();
	if (pos.inCheck()) {
		for (u64 i = 0; i < num; ++i) {
			pms = &legalMoves[0];
			pms = generateMoves<Evasion>(pms, pos);
		}
	}
	else {
		for (u64 i = 0; i < num; ++i) {
			pms = &legalMoves[0];
			pms = generateMoves<CapturePlusPro>(pms, pos);
			pms = generateMoves<NonCaptureMinusPro>(pms, pos);
			pms = generateMoves<Drop>(pms, pos);
			//			pms = generateMoves<PseudoLegal>(pms, pos);
			//			pms = generateMoves<Legal>(pms, pos);
		}
	}
	const int elapsed = t.elapsed();
	std::cout << "elapsed = " << elapsed << " [msec]" << std::endl;
	if (elapsed != 0) {
		std::cout << "times/s = " << num * 1000 / elapsed << " [times/sec]" << std::endl;
	}
	const ptrdiff_t count = pms - &legalMoves[0];
	std::cout << "num of moves = " << count << std::endl;
	for (int i = 0; i < count; ++i) {
		std::cout << legalMoves[i].move.toCSA() << ", ";
	}
	std::cout << std::endl;
}
#endif

#ifdef NDEBUG
const std::string MyName = "Kifuwarapery(Apery_Twig_SDT3)";
#else
const std::string MyName = "Kifuwarapery(Apery) Debug Build";
#endif


UsiLoop::UsiLoop()
{
}

void UsiLoop::Mainloop(int argc, char* argv[])
{
	Position pos(DefaultStartPositionSFEN, Searcher::threads.mainThread(), Searcher::thisptr);

	std::string cmd;
	std::string token;

#if defined MPI_LEARN
	boost::mpi::environment  env(argc, argv);
	boost::mpi::communicator world;
	if (world.rank() != 0) {
		learn(pos, env, world);
		return;
	}
#endif

	for (int i = 1; i < argc; ++i)
	{
		cmd += std::string(argv[i]) + " ";
	}

	do {
		if (argc == 1)
		{
			std::getline(std::cin, cmd);
		}

		std::istringstream ssCmd(cmd);

		ssCmd >> std::skipws >> token;

		UsiOperation usiOperation;

		if (token == "quit" || token == "stop" || token == "ponderhit" || token == "gameover") {
			if (token != "ponderhit" || Searcher::signals.stopOnPonderHit) {
				Searcher::signals.stop = true;
				Searcher::threads.mainThread()->notifyOne();
			}
			else {
				Searcher::limits.ponder = false;
			}

			if (token == "ponderhit" && Searcher::limits.moveTime != 0) {
				Searcher::limits.moveTime += Searcher::searchTimer.elapsed();
			}
		}
		else if (token == "usinewgame") {
			Searcher::tt.clear();
#if defined INANIWA_SHIFT
			inaniwaFlag = NotInaniwa;
#endif
#if defined BISHOP_IN_DANGER
			bishopInDangerFlag = NotBishopInDanger;
#endif
			for (int i = 0; i < 100; ++i)
			{
				g_randomTimeSeed(); // 最初は乱数に偏りがあるかも。少し回しておく。
			}
		}
		else if (token == "usi") {
			SYNCCOUT << "id name " << MyName
				<< "\nid author Hiraoka Takuya"
				<< "\n" << Searcher::options
				<< "\nusiok" << SYNCENDL;
		}
		else if (token == "go") { usiOperation.go(pos, ssCmd); }
		else if (token == "isready") { SYNCCOUT << "readyok" << SYNCENDL; }
		else if (token == "position") { usiOperation.setPosition(pos, ssCmd); }
		else if (token == "setoption") { Searcher::setOption(ssCmd); }
#if defined LEARN
		else if (token == "l") {
			auto learner = std::unique_ptr<Learner>(new Learner);
#if defined MPI_LEARN
			learner->learn(pos, env, world);
#else
			learner->learn(pos, ssCmd);
#endif
		}
#endif
#if !defined MINIMUL
		// 以下、デバッグ用
		else if (token == "bench") { benchmark(pos); }
		else if (token == "d") { pos.print(); }
		else if (token == "s") { measureGenerateMoves(pos); }
		else if (token == "t") { std::cout << pos.mateMoveIn1Ply().toCSA() << std::endl; }
		else if (token == "b") { makeBook(pos, ssCmd); }
#endif
		else { SYNCCOUT << "unknown command: " << cmd << SYNCENDL; }
	} while (token != "quit" && argc == 1);

	if (Searcher::options["Write_Synthesized_Eval"])
	{
		// シンセサイズド評価を書き出します。
		Evaluater::writeSynthesized(Searcher::options["Eval_Dir"]);
	}

	Searcher::threads.waitForThinkFinished();
}
