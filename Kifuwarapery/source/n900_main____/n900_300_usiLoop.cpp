#include <iostream>
#include "../../header/n165_movStack/n165_400_move.hpp"
#include "../../header/n165_movStack/n165_420_convMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_030_makePromoteMove.hpp"
#include "../../header/n407_moveGen_/n407_800_moveGenerator200.hpp"
#include "../../header/n407_moveGen_/n407_900_moveList.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../../header/n480_tt______/n480_300_tt.hpp"
#include "../../header/n520_evaluate/n520_500_kkKkpKppStorage1.hpp"
#include "../../header/n600_book____/n600_500_book.hpp"
#include "../../header/n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../../header/n680_egOption/n680_245_engineOption.hpp"
#include "../../header/n720_usi_____/n720_260_usiOperation.hpp"
#include "../../header/n720_usi_____/n720_300_benchmark.hpp"
#include "../../header/n810_learn___/n810_500_learner.hpp"
#include "../../header/n885_searcher/n885_040_rucksack.hpp"
#include "..\..\header/n900_main____\n900_200_usiItem.hpp"
#include "..\..\header\n900_main____\n900_300_usiLoop.hpp"



#ifdef NDEBUG
const std::string MyName = "Kifuwarapery(Apery_Twig_SDT3)";
#else
const std::string MyName = "Kifuwarapery(Apery) Debug Build";
#endif


UsiLoop::UsiLoop()
{
}

void UsiLoop::Mainloop(
	int			argc,		// argv配列の要素数
	char*		argv[],		// トークンが入っている
	Rucksack&	rucksack
	)
{
	Position pos(g_DefaultStartPositionSFEN, rucksack.m_ownerHerosPub.GetFirstCaptain(), &rucksack);

	std::string cmdStr;
	std::string token;

#if defined MPI_LEARN
	boost::mpi::environment  env(argc, argv);
	boost::mpi::communicator world;
	if (world.m_rank() != 0) {
		learn(GetPos, env, world);
		return;
	}
#endif

	// コマンドライン引数を、スペース区切りでつなげるぜ☆（＾ｑ＾）
	for (int i = 1; i < argc; ++i)
	{
		cmdStr += std::string(argv[i]) + " ";
	}

	do {
		if (argc == 1) // コマンドライン引数が１個のときは
		{
			std::getline(std::cin, cmdStr);
		}

		// 文字列ストリームに１回変えて☆
		std::istringstream issCmd(cmdStr);

		// 空白を読み飛ばしている☆？
		issCmd >> std::skipws >> token;

		UsiOperation usiOperation;

		if (
			token == "quit" ||
			token == "stop" ||
			token == "ponderhit" ||
			token == "gameover"
		) {
			if (token != "ponderhit" ||
				rucksack.m_signals.m_stopOnPonderHit
			) {
				rucksack.m_signals.m_stop = true;
				rucksack.m_ownerHerosPub.GetFirstCaptain()->NotifyOne();
			}
			else {
				rucksack.m_limits.m_ponder = false;
			}

			if (token == "ponderhit" && rucksack.m_limits.GetMoveTime() != 0) {
				rucksack.m_limits.IncreaseMoveTime( rucksack.m_stopwatch.GetElapsed());
			}
		}
		else if (token == "usinewgame") {
			rucksack.m_tt.Clear();
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
				<< "\n" << rucksack.m_engineOptions
				<< "\nusiok" << SYNCENDL;
		}
		else if (token == "go") {
			usiOperation.Go(pos, issCmd);
		}
		else if (token == "isready") {
			SYNCCOUT << "readyok" << SYNCENDL;
		}
		else if (token == "position") {
			usiOperation.SetPosition(pos, issCmd);
		}
		else if (token == "setoption") {
			rucksack.SetOption(issCmd);
		}
#if defined LEARN
		else if (token == "l") {
			auto learner = std::unique_ptr<Learner>(new Learner);
#if defined MPI_LEARN
			learner->learn(GetPos, env, world);
#else
			learner->learn(GetPos, issCmd);
#endif
		}
#endif
#if !defined MINIMUL
		// 以下、デバッグ用
		else if (token == "bench") { Benchmark(pos); }
		else if (token == "d") { pos.Print(); }
		else if (token == "s") { UsiItem::MeasureGenerateMoves(pos); }
		else if (token == "t") { std::cout <<
			(
				pos.GetTurn()==Color::Black
				?
				pos.GetMateMoveIn1Ply<Color::Black,Color::White>().ToCSA()
				:
				pos.GetMateMoveIn1Ply<Color::White,Color::Black>().ToCSA()
			)			
			<< std::endl; }
		else if (token == "b") { MakeBook(pos, issCmd); }
#endif
		else { SYNCCOUT << "unknown command: " << cmdStr << SYNCENDL; }
	} while (token != "quit" && argc == 1);

	//────────────────────────────────────────────────────────────────────────────────

	// 評価値ファイルを書き出す指定なら
	if (rucksack.m_engineOptions["Write_Synthesized_Eval"])
	{
		// シンセサイズド評価を書き出します。
		KkKkpKppStorage1::WriteSynthesized(rucksack.m_engineOptions["Eval_Dir"]);
	}

	//────────────────────────────────────────────────────────────────────────────────

	rucksack.m_ownerHerosPub.WaitForThinkFinished();
}
