#include <iostream>
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_600_utilMove.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n360_genMove_/n360_800_moveGenerator200.hpp"
#include "../../header/n360_genMove_/n360_900_moveList.hpp"
#include "../../header/n440_movStack/n440_500_movePicker.hpp"
#include "../../header/n480_tt______/n480_300_tt.hpp"
#include "../../header/n520_evaluate/n520_500_kkKkpKppStorage1.hpp"
#include "../../header/n600_book____/n600_500_book.hpp"
#include "../../header/n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../../header/n680_egOption/n680_245_engineOption.hpp"
#include "../../header/n720_usi_____/n720_260_usiOperation.hpp"
#include "../../header/n720_usi_____/n720_300_benchmark.hpp"
#include "../../header/n800_learn___/n800_500_learner.hpp"
#include "../../header/n885_searcher/n885_500_searcher.hpp"
#include "..\..\header\n900_main____\n900_300_usiLoop.hpp"


#if !defined MINIMUL
// for debug
// 指し手生成の速度を計測
void measureGenerateMoves(const Position& pos) {
	pos.Print();

	MoveStack legalMoves[g_MaxLegalMoves];
	for (int i = 0; i < g_MaxLegalMoves; ++i)
	{
		legalMoves[i].m_move = UtilMove::GetMoveNone();
	}
	MoveStack* pms = &legalMoves[0];
	const u64 num = 5000000;
	Time t = Time::CurrentTime();
	if (pos.InCheck()) {
		for (u64 i = 0; i < num; ++i) {
			pms = &legalMoves[0];
			pms = MoveGenerator200::GenerateMoves_2<Evasion>(pms, pos);
		}
	}
	else {
		for (u64 i = 0; i < num; ++i) {
			pms = &legalMoves[0];
			pms = MoveGenerator200::GenerateMoves_2<CapturePlusPro>(pms, pos);
			pms = MoveGenerator200::GenerateMoves_2<NonCaptureMinusPro>(pms, pos);
			pms = MoveGenerator200::GenerateMoves_2<Drop>(pms, pos);
			//			pms = generateMoves<PseudoLegal>(pms, pos);
			//			pms = generateMoves<Legal>(pms, pos);
		}
	}
	const int elapsed = t.Elapsed();
	std::cout << "elapsed = " << elapsed << " [msec]" << std::endl;
	if (elapsed != 0) {
		std::cout << "times/s = " << num * 1000 / elapsed << " [times/sec]" << std::endl;
	}
	const ptrdiff_t count = pms - &legalMoves[0];
	std::cout << "num of moves = " << count << std::endl;
	for (int i = 0; i < count; ++i) {
		std::cout << legalMoves[i].m_move.ToCSA() << ", ";
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

void UsiLoop::Mainloop(int argc, char* argv[], Searcher& searcher)
{
	Position pos(g_DefaultStartPositionSFEN, searcher.m_threads.GetMainThread(), &searcher);

	std::string cmd;
	std::string token;

#if defined MPI_LEARN
	boost::mpi::environment  env(argc, argv);
	boost::mpi::communicator world;
	if (world.m_rank() != 0) {
		learn(GetPos, env, world);
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

		if (
			token == "quit" ||
			token == "stop" ||
			token == "ponderhit" ||
			token == "gameover"
		) {
			if (token != "ponderhit" ||
				searcher.m_signals.m_stopOnPonderHit
			) {
				searcher.m_signals.m_stop = true;
				searcher.m_threads.GetMainThread()->NotifyOne();
			}
			else {
				searcher.m_limits.m_ponder = false;
			}

			if (token == "ponderhit" && searcher.m_limits.m_moveTime != 0) {
				searcher.m_limits.m_moveTime += searcher.m_searchTimer.Elapsed();
			}
		}
		else if (token == "usinewgame") {
			searcher.m_tt.Clear();
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
				<< "\nid author (Derivation)Takahashi Satoshi (Base)Hiraoka Takuya"
				<< "\n" << searcher.m_engineOptions
				<< "\nusiok" << SYNCENDL;
		}
		else if (token == "go") {
			usiOperation.Go(pos, ssCmd);
		}
		else if (token == "isready") {
			SYNCCOUT << "readyok" << SYNCENDL;
		}
		else if (token == "position") {
			usiOperation.SetPosition(pos, ssCmd);
		}
		else if (token == "setoption") {
			searcher.SetOption(ssCmd);
		}
#if defined LEARN
		else if (token == "l") {
			auto learner = std::unique_ptr<Learner>(new Learner);
#if defined MPI_LEARN
			learner->learn(GetPos, env, world);
#else
			learner->learn(GetPos, ssCmd);
#endif
		}
#endif
#if !defined MINIMUL
		// 以下、デバッグ用
		else if (token == "bench") { Benchmark(pos); }
		else if (token == "d") { pos.Print(); }
		else if (token == "s") { measureGenerateMoves(pos); }
		else if (token == "t") { std::cout << pos.GetMateMoveIn1Ply().ToCSA() << std::endl; }
		else if (token == "b") { MakeBook(pos, ssCmd); }
#endif
		else { SYNCCOUT << "unknown command: " << cmd << SYNCENDL; }
	} while (token != "quit" && argc == 1);

	//────────────────────────────────────────────────────────────────────────────────

	// 評価値ファイルを書き出す指定なら
	if (searcher.m_engineOptions["Write_Synthesized_Eval"])
	{
		// シンセサイズド評価を書き出します。
		KkKkpKppStorage1::WriteSynthesized(searcher.m_engineOptions["Eval_Dir"]);
	}

	//────────────────────────────────────────────────────────────────────────────────

	searcher.m_threads.WaitForThinkFinished();
}
