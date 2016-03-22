#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n280_move____/n280_100_generateMoves.hpp"
#include "../../header/n280_move____/n280_150_movePicker.hpp"
#include "../../header/n320_operate_/n320_100_book.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n320_operate_/n320_250_usi.hpp"
#include "../../header/n320_operate_/n320_260_usiOperation.hpp"
#include "../../header/n320_operate_/n320_300_benchmark.hpp"
#include "../../header/n320_operate_/n320_400_learner.hpp"

namespace {
	void onThreads  (Searcher* s, const UsiOptionable&    ) { s->threads.readUSIOptions(s); }
	void onHashSize (Searcher* s, const UsiOptionable& opt) { s->tt.setSize(opt); }
	void onClearHash(Searcher* s, const UsiOptionable&    ) { s->tt.clear(); }
	void onEvalDir  (Searcher*  , const UsiOptionable& opt) {
		std::unique_ptr<Evaluater>(new Evaluater)->init(opt, true);
	}
}

bool CaseInsensitiveLess::operator () (const std::string& s1, const std::string& s2) const {
	for (size_t i = 0; i < s1.size() && i < s2.size(); ++i) {
		const int c1 = tolower(s1[i]);
		const int c2 = tolower(s2[i]);

		if (c1 != c2) {
			return c1 < c2;
		}
	}
	return s1.size() < s2.size();
}

namespace {
	// 論理的なコア数の取得
	inline int cpuCoreCount() {
        // todo: boost::thread::physical_concurrency() を使うこと。
		// std::thread::hardware_concurrency() は 0 を返す可能性がある。
		return std::max(static_cast<int>(std::thread::hardware_concurrency()/2), 1);
	}

}

void OptionsMap::init(Searcher* s) {
	(*this)["USI_Hash"]                    = USIOption(256, 1, 65536, onHashSize , s);
	(*this)["Clear_Hash"]                  = USIOption(               onClearHash, s);
	(*this)["Book_File"]                   = USIOption("book/20150503/book.bin");
	(*this)["Best_Book_Move"]              = USIOption(false);
	(*this)["OwnBook"]                     = USIOption(true);
	(*this)["Min_Book_Ply"]                = USIOption(SHRT_MAX, 0, SHRT_MAX);
	(*this)["Max_Book_Ply"]                = USIOption(SHRT_MAX, 0, SHRT_MAX);
	(*this)["Min_Book_Score"]              = USIOption(-180, -ScoreInfinite, ScoreInfinite);
	(*this)["Eval_Dir"]                    = USIOption("20151105", onEvalDir);
	(*this)["Write_Synthesized_Eval"]      = USIOption(false);
	(*this)["USI_Ponder"]                  = USIOption(true);
	(*this)["Byoyomi_Margin"]              = USIOption(500, 0, INT_MAX);
	(*this)["MultiPV"]                     = USIOption(1, 1, MaxLegalMoves);
	(*this)["Skill_Level"]                 = USIOption(20, 0, 20);
	(*this)["Max_Random_Score_Diff"]       = USIOption(0, 0, ScoreMate0Ply);
	(*this)["Max_Random_Score_Diff_Ply"]   = USIOption(40, 0, SHRT_MAX);
	(*this)["Emergency_Move_Horizon"]      = USIOption(40, 0, 50);
	(*this)["Emergency_Base_Time"]         = USIOption(200, 0, 30000);
	(*this)["Emergency_Move_Time"]         = USIOption(70, 0, 5000);
	(*this)["Slow_Mover"]                  = USIOption(100, 10, 1000);
	(*this)["Minimum_Thinking_Time"]       = USIOption(1500, 0, INT_MAX);
	(*this)["Max_Threads_per_Split_Point"] = USIOption(5, 4, 8, onThreads, s);
	(*this)["Threads"]                     = USIOption(cpuCoreCount(), 1, MaxThreads, onThreads, s);
	(*this)["Use_Sleeping_Threads"]        = USIOption(false);
#if defined BISHOP_IN_DANGER
	(*this)["Danger_Demerit_Score"]        = USIOption(700, SHRT_MIN, SHRT_MAX);
#endif
}

UsiOptionable::UsiOptionable(const char* v, Fn* f, Searcher* s) :
	type_("string"), min_(0), max_(0), onChange_(f), searcher_(s)
{
	defaultValue_ = currentValue_ = v;
}

UsiOptionable::UsiOptionable(const bool v, Fn* f, Searcher* s) :
	type_("check"), min_(0), max_(0), onChange_(f), searcher_(s)
{
	defaultValue_ = currentValue_ = (v ? "true" : "false");
}

UsiOptionable::UsiOptionable(Fn* f, Searcher* s) :
	type_("button"), min_(0), max_(0), onChange_(f), searcher_(s) {}

UsiOptionable::UsiOptionable(const int v, const int min, const int max, Fn* f, Searcher* s)
	: type_("spin"), min_(min), max_(max), onChange_(f), searcher_(s)
{
	std::ostringstream ss;
	ss << v;
	defaultValue_ = currentValue_ = ss.str();
}

USIOption::USIOption(const char* v, Fn* f, Searcher* s) : UsiOptionable(v,f,s)
{
}

USIOption::USIOption(const bool v, Fn* f, Searcher* s) : UsiOptionable(v,f,s)
{
}

USIOption::USIOption(Fn* f, Searcher* s) : UsiOptionable(f,s)
{
}

USIOption::USIOption(const int v, const int min, const int max, Fn* f, Searcher* s) : UsiOptionable(v,min,max,f,s)
{
}


UsiOptionable& UsiOptionable::operator = (const std::string& v) {
	assert(!type_.empty());

	if ((type_ != "button" && v.empty())
		|| (type_ == "check" && v != "true" && v != "false")
		|| (type_ == "spin" && (atoi(v.c_str()) < min_ || max_ < atoi(v.c_str()))))
	{
		return *this;
	}

	if (type_ != "button") {
		currentValue_ = v;
	}

	if (onChange_ != nullptr) {
		(*onChange_)(searcher_, *this);
	}

	return *this;
}

std::ostream& operator << (std::ostream& os, const OptionsMap& om) {
	for (auto& elem : om) {
		const UsiOptionable& o = elem.second;
		os << "\noption name " << elem.first << " type " << o.GetType();
		if (o.GetType() != "button") {
			os << " default " << o.GetDefaultValue();
		}

		if (o.GetType() == "spin") {
			os << " min " << o.GetMin() << " max " << o.GetMax();
		}
	}
	return os;
}


void Searcher::setOption(std::istringstream& ssCmd) {
	std::string token;
	std::string name;
	std::string value;

	ssCmd >> token; // "name" が入力されるはず。

	ssCmd >> name;
	// " " が含まれた名前も扱う。
	while (ssCmd >> token && token != "value") {
		name += " " + token;
	}

	ssCmd >> value;
	// " " が含まれた値も扱う。
	while (ssCmd >> token) {
		value += " " + token;
	}

	if (!options.isLegalOption(name)) {
		std::cout << "No such option: " << name << std::endl;
	}
	else {
		options[name] = value;
	}
}

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

void Searcher::doUSICommandLoop(int argc, char* argv[]) {
	Position pos(DefaultStartPositionSFEN, threads.mainThread(), thisptr);

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
			if (token != "ponderhit" || signals.stopOnPonderHit) {
				signals.stop = true;
				threads.mainThread()->notifyOne();
			}
			else {
				limits.ponder = false;
			}

			if (token == "ponderhit" && limits.moveTime != 0) {
				limits.moveTime += searchTimer.elapsed();
			}
		}
		else if (token == "usinewgame") {
			tt.clear();
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
		else if (token == "usi"      ) { SYNCCOUT << "id name " << MyName
												  << "\nid author Hiraoka Takuya"
												  << "\n" << options
												  << "\nusiok" << SYNCENDL; }
		else if (token == "go"       ) { usiOperation.go(pos, ssCmd); }
		else if (token == "isready"  ) { SYNCCOUT << "readyok" << SYNCENDL; }
		else if (token == "position" ) { usiOperation.setPosition(pos, ssCmd); }
		else if (token == "setoption") { setOption(ssCmd); }
#if defined LEARN
		else if (token == "l"        ) {
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
		else if (token == "bench"    ) { benchmark(pos); }
		else if (token == "d"        ) { pos.print(); }
		else if (token == "s"        ) { measureGenerateMoves(pos); }
		else if (token == "t"        ) { std::cout << pos.mateMoveIn1Ply().toCSA() << std::endl; }
		else if (token == "b"        ) { makeBook(pos, ssCmd); }
#endif
		else                           { SYNCCOUT << "unknown command: " << cmd << SYNCENDL; }
	} while (token != "quit" && argc == 1);

	if (options["Write_Synthesized_Eval"])
	{
		// シンセサイズド評価を書き出します。
		Evaluater::writeSynthesized(options["Eval_Dir"]);
	}

	threads.waitForThinkFinished();
}
