#pragma once

#if defined LEARN

#include <atomic>		// std::atomic
#include <set>			// std::set
#include <algorithm>	// std::min
#include "../n110_square__/n110_500_convSquare.hpp"
#include "../n220_position/n220_750_charToPieceUSI.hpp"
#include "../n760_thread__/n760_400_herosPub.hpp"
#include "../n800_stpWatch/n800_100_stopwatch.hpp"

#include "../n830_evaluate/n830_400_KkKkpKppStorageBase.hpp"
#include "../n830_evaluate/n830_700_evaluation09.hpp"
#include "../n850_learn___/n850_100_rawEvaluater.hpp"
#include "../n850_learn___/n850_200_parse2Data.hpp"
#include "../n850_learn___/n850_300_bookMoveData.hpp"

#include "../n885_searcher/n885_040_rucksack.hpp"// FIXME:
//class Rucksack;


#if 0
#define PRINT_PV(x) x
#else
#define PRINT_PV(x)
#endif




//────────────────────────────────────────────────────────────────────────────────────────────────────
// float 型の atomic 加算
//────────────────────────────────────────────────────────────────────────────────────────────────────
inline float atomicAdd(std::atomic<float> &x, const float diff) {
	float old = x.load(std::memory_order_consume);
	float desired = old + diff;
	while (!x.compare_exchange_weak(old, desired, std::memory_order_release, std::memory_order_consume))
		desired = old + diff;
	return desired;
}

//────────────────────────────────────────────────────────────────────────────────────────────────────
// float 型の atomic 減算
//────────────────────────────────────────────────────────────────────────────────────────────────────
inline float atomicSub(std::atomic<float> &x, const float diff) {
	float old = x.load(std::memory_order_consume);
	float desired = old - diff;
	while (!x.compare_exchange_weak(old, desired, std::memory_order_release, std::memory_order_consume))
		desired = old - diff;
	return desired;
}

//────────────────────────────────────────────────────────────────────────────────────────────────────
// 演算子
//────────────────────────────────────────────────────────────────────────────────────────────────────
RawEvaluater& operator += (RawEvaluater& lhs, RawEvaluater& rhs) {
	for (auto lit = &(***std::begin(lhs.kpp_raw)), rit = &(***std::begin(rhs.kpp_raw)); lit != &(***std::end(lhs.kpp_raw)); ++lit, ++rit)
		*lit += *rit;
	for (auto lit = &(***std::begin(lhs.kkp_raw)), rit = &(***std::begin(rhs.kkp_raw)); lit != &(***std::end(lhs.kkp_raw)); ++lit, ++rit)
		*lit += *rit;
	for (auto lit = &(** std::begin(lhs.kk_raw )), rit = &(** std::begin(rhs.kk_raw )); lit != &(** std::end(lhs.kk_raw )); ++lit, ++rit)
		*lit += *rit;

	return lhs;
}

//────────────────────────────────────────────────────────────────────────────────────────────────────
// kpp_raw, kkp_raw, kk_raw の値を低次元の要素に与える。
//────────────────────────────────────────────────────────────────────────────────────────────────────
inline void lowerDimension(
	KkKkpKppStorageBase<std::array<std::atomic<float>, 2>,
	std::array<std::atomic<float>, 2>,
	std::array<std::atomic<float>, 2> >& base,
	const RawEvaluater& raw
){
#define FOO(indices, oneArray, GetSum)										\
	for (auto indexAndWeight : indices) {								\
		if (indexAndWeight.first == std::numeric_limits<ptrdiff_t>::max()) break; \
		if (0 <= indexAndWeight.first) {								\
			atomicAdd((*oneArray( indexAndWeight.first))[0], GetSum[0] * indexAndWeight.second / base.MaxWeight()); \
			atomicAdd((*oneArray( indexAndWeight.first))[1], GetSum[1] * indexAndWeight.second / base.MaxWeight()); \
		}																\
		else {															\
			atomicSub((*oneArray(-indexAndWeight.first))[0], GetSum[0] * indexAndWeight.second / base.MaxWeight()); \
			atomicAdd((*oneArray(-indexAndWeight.first))[1], GetSum[1] * indexAndWeight.second / base.MaxWeight()); \
		}																\
	}

#if defined _OPENMP
#pragma omp parallel
#endif

	// KPP
	{
#ifdef _OPENMP
#pragma omp for
#endif
		for (int ksq = I9; ksq < SquareNum; ++ksq) {
			std::pair<ptrdiff_t, int> indices[g_KPPIndicesMax];
			for (int i = 0; i < fe_end; ++i) {
				for (int j = 0; j < fe_end; ++j) {
					base.CreateKppIndices(indices, static_cast<Square>(ksq), i, j);
					FOO(indices, base.GetKppOneArrayFirst, raw.kpp_raw[ksq][i][j]);
				}
			}
		}
	}
	// KKP
	{
#ifdef _OPENMP
#pragma omp for
#endif
		for (int ksq0 = I9; ksq0 < SquareNum; ++ksq0) {
			std::pair<ptrdiff_t, int> indices[g_KKPIndicesMax];
			for (Square ksq1 = I9; ksq1 < SquareNum; ++ksq1) {
				for (int i = 0; i < fe_end; ++i) {
					base.CreateKkpIndices(indices, static_cast<Square>(ksq0), ksq1, i);
					FOO(indices, base.GetKkpOneArrayFirst, raw.kkp_raw[ksq0][ksq1][i]);
				}
			}
		}
	}
	// KK
	{
#ifdef _OPENMP
#pragma omp for
#endif
		for (int ksq0 = I9; ksq0 < SquareNum; ++ksq0) {
			std::pair<ptrdiff_t, int> indices[g_KKIndicesMax];
			for (Square ksq1 = I9; ksq1 < SquareNum; ++ksq1) {
				base.CreateKkIndices(indices, static_cast<Square>(ksq0), ksq1);
				FOO(indices, base.GetKkOneArrayFirst, raw.kk_raw[ksq0][ksq1]);
			}
		}
	}
#undef FOO
}


class Learner {
public:

	// 学習☆
	void learn(Position& pos, std::istringstream& ssCmd) {

		this->eval_.Init(
			pos.GetRucksack()->m_engineOptions["Eval_Dir"],
			false
			);

		// ゲーム番号
		s64 gameNum;

		// 棋譜ファイル名
		std::string recordFileName;

		// 黒棋譜ファイル名
		std::string blackRecordFileName;

		// 白棋譜ファイル名
		std::string whiteRecordFileName;

		// スレッド番号
		size_t threadNum;

		// 最大値☆？
		s64 updateMax;

		// 最小値☆？
		s64 updateMin;

		// 棋譜ファイル名☆
		ssCmd >> recordFileName;

		// 黒棋譜ファイル名☆
		ssCmd >> blackRecordFileName;

		// 白棋譜ファイル名☆
		ssCmd >> whiteRecordFileName;

		// ゲーム番号☆
		ssCmd >> gameNum;

		// スレッド番号☆
		ssCmd >> threadNum;

		// 最小深さ☆
		ssCmd >> this->minDepth_;

		// 最大深さ☆
		ssCmd >> this->maxDepth_;

		// ステップ番号☆
		ssCmd >> this->stepNum_;

		// 反復のためのゲーム番号☆
		ssCmd >> this->gameNumForIteration_;

		// 最大☆
		ssCmd >> updateMax;

		// 最小☆
		ssCmd >> updateMin;

		// ペナルティ☆
		ssCmd >> this->usePenalty_;

		std::cout << "\n"; // ファイルにログをリダイレクトしたとき、追記の場合はまずは改行した方が見易い。

		if (updateMax < 0 || 64 < updateMax) {
			updateMax = 64; // 乱数が 64 bit なので、bit count 方式だと 64 が上限。
			std::cout << "you can set update_max [1, 64]" << std::endl;
		}
		if (updateMin < 0 || updateMax < updateMin) {
			updateMin = updateMax;
			std::cout << "you can set update_min [1, update_max]" << std::endl;
		}
		std::cout << "record_file: " << recordFileName
				  << "\nblack record_file: " << blackRecordFileName
				  << "\nwhite record_file: " << whiteRecordFileName
				  << "\nread games: " << (gameNum == 0 ? "all" : std::to_string(gameNum))
				  << "\nthread_num: " << threadNum
				  << "\nsearch_depth min, max: " << minDepth_ << ", " << maxDepth_
				  << "\nstep_num: " << stepNum_
				  << "\ngame_num_for_iteration: " << gameNumForIteration_
				  << "\nupdate_max: " << updateMax
				  << "\nupdate_min: " << updateMin
				  << "\nuse_penalty: " << usePenalty_
				  << std::endl;
		this->updateMaxMask_ = (UINT64_C(1) << updateMax) - 1;
		this->updateMinMask_ = (UINT64_C(1) << updateMin) - 1;
		setUpdateMask(this->stepNum_);
		readBook(pos, recordFileName, blackRecordFileName, whiteRecordFileName, gameNum);
		// 既に 1 つのSearcher, Positionが立ち上がっているので、指定した数 - 1 の Searcher, Position を立ち上げる。
		threadNum = std::max<size_t>(0, threadNum - 1);
		std::vector<Rucksack> rucksacks(threadNum);
		for (auto& rucksack : rucksacks) {

			rucksack.Init();

			setLearnOptions(rucksack);

			this->positions_.push_back(
				Position(
					g_DefaultStartPositionSFEN,
					rucksack.m_ownerHerosPub.GetFirstCaptain(),
					&rucksack//.thisptr
				)
			);

			this->mts_.push_back(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()));
			// ここでデフォルトコンストラクタでpush_backすると、
			// 一時オブジェクトのParse2Dataがスタックに出来ることでプログラムが落ちるので、コピーコンストラクタにする。
			parse2Datum_.push_back(this->parse2Data_);
		}
		setLearnOptions(*pos.GetRucksack());
		this->mt_ = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
		this->mt64_ = std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count());
		for (int i = 0; ; ++i) {
			std::cout << "iteration " << i << std::endl;
			std::cout << "parse1 start" << std::endl;
			learnParse1(pos);
			std::cout << "parse2 start" << std::endl;
			learnParse2(pos);
		}
	}

private:
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	// 学習に使う棋譜から、手と手に対する補助的な情報を付けでデータ保持する。
	// 50000局程度に対して10秒程度で終わるからシングルコアで良い。
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void setLearnMoves(
		Position& pos,
		std::set<std::pair<Key, Move> >& dict,
		std::string& s0,
		std::string& s1,
		const std::array<bool, g_COLOR_NUM>& useTurnMove
	){
		// スタックに積む☆？
		bookMovesDatum_.push_back(std::vector<BookMoveData>());

		// ベース☆？
		BookMoveData bmdBase[g_COLOR_NUM];

		// 指し手クリアー☆？
		bmdBase[Black].move = bmdBase[White].move = g_MOVE_NONE;

		// 文字列☆？
		std::stringstream textA(s0);

		// 文字列使いまわし☆？
		std::string elem;
		textA >> elem; // 対局番号
		textA >> elem; // 対局日
		bmdBase[Black].date = bmdBase[White].date = elem;
		textA >> elem; // 先手名
		bmdBase[Black].player = elem;
		textA >> elem; // 後手名
		bmdBase[White].player = elem;
		textA >> elem; // 引き分け勝ち負け
		bmdBase[Black].winner = (elem == "1");
		bmdBase[White].winner = (elem == "2");

		pos.Set(
			g_DefaultStartPositionSFEN,
			pos.GetRucksack()->m_ownerHerosPub.GetFirstCaptain()
		);

		StateStackPtr setUpStates = StateStackPtr(new std::stack<StateInfo>());
		UsiOperation usiOperation;
		while (true) {
			const std::string moveStrCSA = s1.substr(0, 6);
			const Move move = usiOperation.CsaToMove(pos, moveStrCSA);
			// 指し手の文字列のサイズが足りなかったり、反則手だったりすれば move.isNone() == true となるので、break する。
			if (move.IsNone())
				break;
			BookMoveData bmd = bmdBase[pos.GetTurn()];
			bmd.move = move;
			if (useTurnMove[pos.GetTurn()] && dict.find(std::make_pair(pos.GetKey(), move)) == std::end(dict)) {
				// この局面かつこの指し手は初めて見るので、学習に使う。
				bmd.useLearning = true;
				dict.insert(std::make_pair(pos.GetKey(), move));
			}
			else
				bmd.useLearning = false;

			bookMovesDatum_.back().push_back(bmd);
			s1.erase(0, 6);

			setUpStates->push(StateInfo());

			pos.GetTurn() == Color::Black
				?
				pos.DoMove<Color::Black,Color::White>(move, setUpStates->top())
				:
				pos.DoMove<Color::White,Color::Black>(move, setUpStates->top())
				;
		}
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void readBookBody(std::set<std::pair<Key, Move> >& dict, Position& pos, const std::string& record, const std::array<bool, g_COLOR_NUM>& useTurnMove, const s64 gameNum)
	{
		if (record == "-") // "-" なら棋譜ファイルを読み込まない。
			return;
		std::ifstream ifs(record.c_str(), std::ios::binary);
		if (!ifs) {
			std::cout << "I cannot read " << record << std::endl;
			exit(EXIT_FAILURE);
		}
		std::string s0;
		std::string s1;
		// 0 なら全部の棋譜を読む
		s64 tmpGameNum = (gameNum == 0 ? std::numeric_limits<s64>::max() : gameNum);
		for (s64 i = 0; i < tmpGameNum; ++i) {
			std::getline(ifs, s0);
			std::getline(ifs, s1);
			if (!ifs) break;
			setLearnMoves(pos, dict, s0, s1, useTurnMove);
		}
		std::cout << "games existed: " << bookMovesDatum_.size() << std::endl;
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void readBook(Position& pos,
				  const std::string& recordFileName,
				  const std::string& blackRecordFileName,
				  const std::string& whiteRecordFileName, const s64 gameNum)
	{
		std::set<std::pair<Key, Move> > dict;
		readBookBody(dict, pos,      recordFileName, {true , true }, gameNum);
		readBookBody(dict, pos, blackRecordFileName, {true , false}, gameNum);
		readBookBody(dict, pos, whiteRecordFileName, {false, true }, gameNum);
		gameNumForIteration_ = std::min(gameNumForIteration_, bookMovesDatum_.size());
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void setLearnOptions(Rucksack& s) {
		std::string m_engineOptions[] = {"name Threads value 1",
								 "name MultiPV value 1",
								 "name OwnBook value false",
								 "name Max_Random_Score_Diff value 0"};
		for (auto& str : m_engineOptions) {
			std::istringstream is(str);
			s.SetOption(is);
		}
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	template <bool Dump> size_t lockingIndexIncrement() {
		std::unique_lock<Mutex> lock(m_mutex_);
		if (Dump) {
			if      (index_ % 500 == 0) std::cout << index_ << std::endl;
			else if (index_ % 100 == 0) std::cout << "o" << std::flush;
			else if (index_ %  10 == 0) std::cout << "." << std::flush;
		}
		return index_++;
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void learnParse1Body(Position& pos, std::mt19937& mt) {
		std::uniform_int_distribution<Ply> dist(minDepth_, maxDepth_);
		pos.GetRucksack()->m_tt.Clear();
		UsiOperation usiOperation;
		for (size_t i = lockingIndexIncrement<true>(); i < gameNumForIteration_; i = lockingIndexIncrement<true>()) {
			StateStackPtr setUpStates = StateStackPtr(new std::stack<StateInfo>());

			pos.Set(g_DefaultStartPositionSFEN, pos.GetRucksack()->m_ownerHerosPub.GetFirstCaptain());

			auto& gameMoves = bookMovesDatum_[i];
			for (auto& bmd : gameMoves) {
				if (bmd.useLearning) {
					pos.GetRucksack()->m_alpha = -ScoreMaxEvaluate;
					pos.GetRucksack()->m_beta  =  ScoreMaxEvaluate;
					usiOperation.Go(pos, dist(mt), bmd.move);
					const ScoreIndex recordScore = pos.GetRucksack()->m_rootMoves[0].m_score_;
					++moveCount_;
					bmd.otherPVExist = false;
					bmd.pvBuffer.clear();
					if (abs(recordScore) < ScoreMaxEvaluate) {
						int recordIsNth = 0; // 正解の手が何番目に良い手か。0から数える。
						auto& recordPv = pos.GetRucksack()->m_rootMoves[0].m_pv_;

						bmd.pvBuffer.insert(
							std::end(bmd.pvBuffer),
							std::begin(recordPv),
							std::end(recordPv)
						);

						const auto recordPVSize = bmd.pvBuffer.size();
						for (MoveList<N09_LegalAll> ml(pos); !ml.IsEnd(); ++ml) {
							if (ml.GetMove() != bmd.move) {

								pos.GetRucksack()->m_alpha = recordScore - FVWindow;

								pos.GetRucksack()->m_beta  = recordScore + FVWindow;

								usiOperation.Go(pos, dist(mt), ml.GetMove());

								const ScoreIndex GetScore = pos.GetRucksack()->m_rootMoves[0].m_score_;

								if (pos.GetRucksack()->m_alpha < GetScore && GetScore < pos.GetRucksack()->m_beta) {
									auto& pv = pos.GetRucksack()->m_rootMoves[0].m_pv_;
									bmd.pvBuffer.insert(std::end(bmd.pvBuffer), std::begin(pv), std::end(pv));
								}
								if (recordScore < GetScore)
									++recordIsNth;
							}
						}
						bmd.otherPVExist = (recordPVSize != bmd.pvBuffer.size());

						for (int i = recordIsNth; i < PredSize; ++i)
						{
							++predictions_[i];
						}
					}
				}
				setUpStates->push(StateInfo());

				pos.GetTurn() == Color::Black
					?
					pos.DoMove<Color::Black,Color::White>(bmd.move, setUpStates->top())
					:
					pos.DoMove<Color::White,Color::Black>(bmd.move, setUpStates->top())
					;
				
			}
		}
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void learnParse1(Position& pos) {
		Stopwatch t = Stopwatch::CreateStopwatchByCurrentTime();
		// 棋譜をシャッフルすることで、先頭 gameNum_ 個の学習に使うデータをランダムに選ぶ。
		std::shuffle(std::begin(bookMovesDatum_), std::end(bookMovesDatum_), mt_);
		std::cout << "shuffle elapsed: " << t.GetElapsed() / 1000 << "[sec]" << std::endl;
		index_ = 0;
		moveCount_.store(0);
		for (auto& pred : predictions_)
			pred.store(0);
		std::vector<std::thread> m_ownerHerosPub(positions_.size());
		for (size_t i = 0; i < positions_.size(); ++i)
			m_ownerHerosPub[i] = std::thread([this, i] { learnParse1Body(positions_[i], mts_[i]); });
		learnParse1Body(pos, mt_);
		for (auto& thread : m_ownerHerosPub)
			thread.join();

		std::cout << "\nGames = " << bookMovesDatum_.size()
				  << "\nTotal Moves = " << moveCount_
				  << "\nPrediction = ";
		for (auto& pred : predictions_)
			std::cout << static_cast<double>(pred.load()*100) / moveCount_.load() << ", ";
		std::cout << std::endl;
		std::cout << "parse1 elapsed: " << t.GetElapsed() / 1000 << "[sec]" << std::endl;
	}


	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	static constexpr double FVPenalty() { return (0.2/static_cast<double>(g_FVScale)); }

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	template <bool UsePenalty, typename T>
	void updateFV(std::array<T, 2>& v, const std::array<std::atomic<float>, 2>& dvRef) {
		std::array<float, 2> dv = {dvRef[0].load(), dvRef[1].load()};
		const int step = count1s(mt64_() & updateMask_);
		for (int i = 0; i < 2; ++i) {
			if (UsePenalty) {
				if      (0 < v[i]) dv[i] -= static_cast<float>(FVPenalty());
				else if (v[i] < 0) dv[i] += static_cast<float>(FVPenalty());
			}

			// T が enum だと 0 になることがある。
			// enum のときは、std::numeric_limits<std::underlying_type<T>::type>::max() などを使う。
			static_assert(std::numeric_limits<T>::max() != 0, "");
			static_assert(std::numeric_limits<T>::min() != 0, "");
			if      (0.0 <= dv[i] && v[i] <= std::numeric_limits<T>::max() - step) v[i] += step;
			else if (dv[i] <= 0.0 && std::numeric_limits<T>::min() + step <= v[i]) v[i] -= step;
		}
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	template <bool UsePenalty>
	void updateEval(const std::string& dirName) {
		for (size_t i = 0; i < eval_.GetKpps_end_index(); ++i)
			updateFV<UsePenalty>(*eval_.GetKppOneArrayFirst(i), *parse2EvalBase_.GetKppOneArrayFirst(i));
		for (size_t i = 0; i < eval_.GetKkps_end_index(); ++i)
			updateFV<UsePenalty>(*eval_.GetKkpOneArrayFirst(i), *parse2EvalBase_.GetKkpOneArrayFirst(i));
		for (size_t i = 0; i < eval_.GetKks_end_index(); ++i)
			updateFV<UsePenalty>(*eval_.GetKkOneArrayFirst(i), *parse2EvalBase_.GetKkOneArrayFirst(i));

		// 学習しないパラメータがある時は、一旦 write() で学習しているパラメータだけ書きこんで、再度読み込む事で、
		// updateFV()で学習しないパラメータに入ったノイズを無くす。
		eval_.Write(dirName);
		eval_.Init(dirName, false);
		g_evalTable.Clear();
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	double sigmoid(const double x) const {
		const double a = 7.0/static_cast<double>(FVWindow);
		const double clipx = std::max(static_cast<double>(-FVWindow), std::min(static_cast<double>(FVWindow), x));
		return 1.0 / (1.0 + exp(-a * clipx));
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	double dsigmoid(const double x) const {
		if (x <= -FVWindow || FVWindow <= x) { return 0.0; }
#if 1
		// 符号だけが大切なので、定数掛ける必要は無い。
		const double a = 7.0/static_cast<double>(FVWindow);
		return a * sigmoid(x) * (1 - sigmoid(x));
#else
		// 定数掛けない方を使う。
		return sigmoid(x) * (1 - sigmoid(x));
#endif
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void setUpdateMask(const int step) {
		const int stepMax = stepNum_;
		const int max = count1s(updateMaxMask_);
		const int min = count1s(updateMinMask_);
		updateMask_ = max - (((max - min)*step+(stepMax>>1))/stepMax);
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void learnParse2Body(Position& pos, Parse2Data& parse2Data) {
		parse2Data.Clear();
		Flashlight m_pFlashlightBox[2];
		for (size_t i = lockingIndexIncrement<false>(); i < gameNumForIteration_; i = lockingIndexIncrement<false>()) {
			StateStackPtr setUpStates = StateStackPtr(new std::stack<StateInfo>());

			pos.Set(
				g_DefaultStartPositionSFEN,
				pos.GetRucksack()->m_ownerHerosPub.GetFirstCaptain()
				);

			auto& gameMoves = bookMovesDatum_[i];
			for (auto& bmd : gameMoves) {
				PRINT_PV(pos.Print());
				if (bmd.useLearning && bmd.otherPVExist) {
					const Color rootColor = pos.GetTurn();
					int recordPVIndex = 0;

					PRINT_PV(std::cout << "recordpv: ");

					for (; !bmd.pvBuffer[recordPVIndex].IsNone(); ++recordPVIndex) {
						PRINT_PV(std::cout << bmd.pvBuffer[recordPVIndex].ToCSA());
						setUpStates->push(StateInfo());

						pos.GetTurn() == Color::Black
							?
							pos.DoMove<Color::Black,Color::White>(bmd.pvBuffer[recordPVIndex], setUpStates->top())
							:
							pos.DoMove<Color::White,Color::Black>(bmd.pvBuffer[recordPVIndex], setUpStates->top())
							;


					}
					// evaluate() の差分計算を無効化する。
					m_pFlashlightBox[0].m_staticEvalRaw.m_p[0][0] = m_pFlashlightBox[1].m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
					Evaluation09 evaluation09;
					const ScoreIndex recordScore =
						(rootColor == pos.GetTurn()
							?
							evaluation09.evaluate(pos, m_pFlashlightBox+1)
							:
							-evaluation09.evaluate(pos, m_pFlashlightBox+1))
						;
					PRINT_PV(std::cout << ", score: " << recordScore << std::endl);
					for (int jj = recordPVIndex - 1; 0 <= jj; --jj) {
						pos.UndoMove(bmd.pvBuffer[jj]);
					}

					std::array<double, 2> sum_dT = {{0.0, 0.0}};
					for (int otherPVIndex = recordPVIndex + 1; otherPVIndex < static_cast<int>(bmd.pvBuffer.size()); ++otherPVIndex) {
						PRINT_PV(std::cout << "otherpv : ");
						for (; !bmd.pvBuffer[otherPVIndex].IsNone(); ++otherPVIndex) {

							PRINT_PV(
								std::cout << bmd.pvBuffer[otherPVIndex].ToCSA()
								);

							setUpStates->push(StateInfo());

							pos.GetTurn() == Color::Black
								?
								pos.DoMove<Color::Black,Color::White>(bmd.pvBuffer[otherPVIndex], setUpStates->top())
								:
								pos.DoMove<Color::White,Color::Black>(bmd.pvBuffer[otherPVIndex], setUpStates->top())
								;

						}
						m_pFlashlightBox[0].m_staticEvalRaw.m_p[0][0] = m_pFlashlightBox[1].m_staticEvalRaw.m_p[0][0] = ScoreNotEvaluated;
						const ScoreIndex score = (rootColor == pos.GetTurn()
							?
							evaluation09.evaluate(pos, m_pFlashlightBox+1)
							:
							-evaluation09.evaluate(pos, m_pFlashlightBox+1))
							;
						const auto diff = score - recordScore;

						const double dsig = dsigmoid(diff);

						std::array<double, 2> dT = {{(rootColor == Black ? dsig : -dsig), dsig}};
						PRINT_PV(std::cout << ", score: " << score << ", dT: " << dT[0] << std::endl);
						sum_dT += dT;
						dT[0] = -dT[0];
						dT[1] = (pos.GetTurn() == rootColor ? -dT[1] : dT[1]);
						parse2Data.params.incParam(pos, dT);
						for (int jj = otherPVIndex - 1; !bmd.pvBuffer[jj].IsNone(); --jj) {
							pos.UndoMove(bmd.pvBuffer[jj]);
						}
					}

					for (int jj = 0; jj < recordPVIndex; ++jj) {
						setUpStates->push(StateInfo());

						pos.GetTurn() == Color::Black
							?
							pos.DoMove<Color::Black,Color::White>(bmd.pvBuffer[jj], setUpStates->top())
							:
							pos.DoMove<Color::White,Color::Black>(bmd.pvBuffer[jj], setUpStates->top())
							;
						

					}
					sum_dT[1] = (pos.GetTurn() == rootColor ? sum_dT[1] : -sum_dT[1]);
					parse2Data.params.incParam(pos, sum_dT);
					for (int jj = recordPVIndex - 1; 0 <= jj; --jj) {
						pos.UndoMove(bmd.pvBuffer[jj]);
					}
				}
				setUpStates->push(StateInfo());

				pos.GetTurn() == Color::Black
					?
					pos.DoMove<Color::Black, Color::White>(bmd.move, setUpStates->top())
					:
					pos.DoMove<Color::White, Color::Black>(bmd.move, setUpStates->top())
					;
			}
		}
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void learnParse2(Position& pos) {

		Stopwatch t;

		for (int step = 1; step <= stepNum_; ++step) {

			t.Restart();

			std::cout << "step " << step << "/" << stepNum_ << " " << std::flush;

			index_ = 0;

			std::vector<std::thread> m_ownerHerosPub(positions_.size());

			for (size_t i = 0; i < positions_.size(); ++i)
				m_ownerHerosPub[i] = std::thread([this, i] { learnParse2Body(positions_[i], parse2Datum_[i]); });
			
			learnParse2Body(pos, parse2Data_);

			for (auto& thread : m_ownerHerosPub)
			{
				thread.join();
			}

			for (auto& parse2 : parse2Datum_) {
				parse2Data_.params += parse2.params;
			}

			parse2EvalBase_.clear();

			lowerDimension(parse2EvalBase_, parse2Data_.params);

			setUpdateMask(step);

			std::cout << "update eval ... " << std::flush;

			if (usePenalty_) updateEval<true >(pos.GetRucksack()->m_engineOptions["Eval_Dir"]);
			else             updateEval<false>(pos.GetRucksack()->m_engineOptions["Eval_Dir"]);

			std::cout << "done" << std::endl;

			std::cout << "parse2 1 step elapsed: " << t.GetElapsed() / 1000 << "[sec]" << std::endl;

			Print();

		}
	}

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	//
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void Print() {

		for (Rank r = Rank9; r < RankNum; ++r) {

			for (File f = FileA; FileI <= f; --f) {

				const Square sq = ConvSquare::FROM_FILE_RANK10(f, r);

				printf("%5d", KkKkpKppStorage1::KPP[B2][f_gold + C2][f_gold + sq][0]);

			}

			printf("\n");

		}

		printf("\n");

		fflush(stdout);

	}

	// なにかのサイズ☆？（＾ｑ＾）
	static const int PredSize = 8;

	// フィーチャー・ベクターの横幅☆？？（＾ｑ＾）
	static const ScoreIndex FVWindow = static_cast<ScoreIndex>(256);

	// ミューテックス☆？（＾ｑ＾）？
	Mutex m_mutex_;

	// インデックス☆？（＾ｑ＾）？
	size_t index_;

	// 最小の深さ☆？（＾ｑ＾）？
	Ply minDepth_;

	// 最大の深さ☆？（＾ｑ＾）？
	Ply maxDepth_;

	// ペナルティを使う☆？（＾～＾）
	bool usePenalty_;

	// 乱数☆？
	std::mt19937 mt_;

	// 乱数☆？
	std::mt19937_64 mt64_;

	// 乱数☆？
	std::vector<std::mt19937> mts_;

	// 局面☆？
	std::vector<Position> positions_;

	// 定跡☆？
	std::vector<std::vector<BookMoveData> > bookMovesDatum_;

	// 指し手の数☆？（＾～＾）
	std::atomic<s64> moveCount_;

	// 予測☆？（＾～＾）
	std::atomic<s64> predictions_[PredSize];

	// ☆？
	Parse2Data parse2Data_;

	// ☆？
	std::vector<Parse2Data> parse2Datum_;

	// ☆？
	KkKkpKppStorageBase<std::array<std::atomic<float>, 2>,
				  std::array<std::atomic<float>, 2>,
				  std::array<std::atomic<float>, 2> > parse2EvalBase_;

	// FIXME: 4GB を超えていて、クラスに入らないぜ☆？（＾ｑ＾）大きくしてしまったか☆
	// ２駒、３駒関係の評価値が入っている☆？
	KkKkpKppStorage1 eval_;

	// ステップ数☆？
	int stepNum_;

	// 反復のためのゲーム番号☆？
	size_t gameNumForIteration_;

	// 最大マスク☆？
	u64 updateMaxMask_;

	// 最小マスク☆？
	u64 updateMinMask_;

	// マスク☆？
	u64 updateMask_;

};

#endif

