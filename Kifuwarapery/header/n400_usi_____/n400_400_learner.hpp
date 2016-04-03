#pragma once

#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n260_evaluate/n260_700_evaluate.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"

#if defined LEARN

#if 0
#define PRINT_PV(x) x
#else
#define PRINT_PV(x)
#endif

struct RawEvaluater {
	std::array<float, 2> kpp_raw[SquareNum][fe_end][fe_end];
	std::array<float, 2> kkp_raw[SquareNum][SquareNum][fe_end];
	std::array<float, 2> kk_raw[SquareNum][SquareNum];

	void incParam(const Position& pos, const std::array<double, 2>& dinc) {
		const Square sq_bk = pos.GetKingSquare(Black);
		const Square sq_wk = pos.GetKingSquare(White);
		const int* GetList0 = pos.GetCplist0();
		const int* GetList1 = pos.GetCplist1();
		const std::array<float, 2> f = {{static_cast<float>(dinc[0] / FVScale), static_cast<float>(dinc[1] / FVScale)}};

		kk_raw[sq_bk][sq_wk] += f;
		for (int i = 0; i < pos.GetNlist(); ++i) {
			const int k0 = GetList0[i];
			const int k1 = GetList1[i];
			for (int j = 0; j < i; ++j) {
				const int l0 = GetList0[j];
				const int l1 = GetList1[j];
				kpp_raw[sq_bk         ][k0][l0] += f;
				kpp_raw[Inverse(sq_wk)][k1][l1][0] -= f[0];
				kpp_raw[Inverse(sq_wk)][k1][l1][1] += f[1];
			}
			kkp_raw[sq_bk][sq_wk][k0] += f;
		}
	}

	void Clear() { memset(this, 0, sizeof(*this)); } // float 型とかだと規格的に 0 は保証されなかった気がするが実用上問題ないだろう。
};

// float 型の atomic 加算
inline float atomicAdd(std::atomic<float> &x, const float diff) {
	float old = x.load(std::memory_order_consume);
	float desired = old + diff;
	while (!x.compare_exchange_weak(old, desired, std::memory_order_release, std::memory_order_consume))
		desired = old + diff;
	return desired;
}
// float 型の atomic 減算
inline float atomicSub(std::atomic<float> &x, const float diff) {
	float old = x.load(std::memory_order_consume);
	float desired = old - diff;
	while (!x.compare_exchange_weak(old, desired, std::memory_order_release, std::memory_order_consume))
		desired = old - diff;
	return desired;
}

RawEvaluater& operator += (RawEvaluater& lhs, RawEvaluater& rhs) {
	for (auto lit = &(***std::begin(lhs.kpp_raw)), rit = &(***std::begin(rhs.kpp_raw)); lit != &(***std::end(lhs.kpp_raw)); ++lit, ++rit)
		*lit += *rit;
	for (auto lit = &(***std::begin(lhs.kkp_raw)), rit = &(***std::begin(rhs.kkp_raw)); lit != &(***std::end(lhs.kkp_raw)); ++lit, ++rit)
		*lit += *rit;
	for (auto lit = &(** std::begin(lhs.kk_raw )), rit = &(** std::begin(rhs.kk_raw )); lit != &(** std::end(lhs.kk_raw )); ++lit, ++rit)
		*lit += *rit;

	return lhs;
}

// kpp_raw, kkp_raw, kk_raw の値を低次元の要素に与える。
inline void lowerDimension(EvaluaterBase<std::array<std::atomic<float>, 2>,
										 std::array<std::atomic<float>, 2>,
										 std::array<std::atomic<float>, 2> >& base, const RawEvaluater& raw)
{
#define FOO(indices, oneArray, sum)										\
	for (auto indexAndWeight : indices) {								\
		if (indexAndWeight.first == std::numeric_limits<ptrdiff_t>::max()) break; \
		if (0 <= indexAndWeight.first) {								\
			atomicAdd((*oneArray( indexAndWeight.first))[0], sum[0] * indexAndWeight.second / base.MaxWeight()); \
			atomicAdd((*oneArray( indexAndWeight.first))[1], sum[1] * indexAndWeight.second / base.MaxWeight()); \
		}																\
		else {															\
			atomicSub((*oneArray(-indexAndWeight.first))[0], sum[0] * indexAndWeight.second / base.MaxWeight()); \
			atomicAdd((*oneArray(-indexAndWeight.first))[1], sum[1] * indexAndWeight.second / base.MaxWeight()); \
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
			std::pair<ptrdiff_t, int> indices[base.KPPIndicesMax];
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
			std::pair<ptrdiff_t, int> indices[base.KKPIndicesMax];
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
			std::pair<ptrdiff_t, int> indices[base.KKIndicesMax];
			for (Square ksq1 = I9; ksq1 < SquareNum; ++ksq1) {
				base.CreateKkIndices(indices, static_cast<Square>(ksq0), ksq1);
				FOO(indices, base.GetKkOneArrayFirst, raw.kk_raw[ksq0][ksq1]);
			}
		}
	}
#undef FOO
}

struct Parse2Data {
	RawEvaluater params;

	void Clear() {
		params.Clear();
	}
};

// 以下のようなフォーマットが入力される。
// <棋譜番号> <日付> <先手名> <後手名> <0:引き分け, 1:先手勝ち, 2:後手勝ち> <総手数> <棋戦名前> <戦形>
// <CSA1行形式の指し手>
//
// (例)
// 1 2003/09/08 羽生善治 谷川浩司 2 126 王位戦 その他の戦型
// 7776FU3334FU2726FU4132KI
struct BookMoveData {
	std::string player; // その手を指した人
	std::string date; // 対局日
	std::vector<Move> pvBuffer; // 正解のPV, その他0のPV, その他1のPV という順に並べる。
								// 間には MoveNone で区切りを入れる。

	Move move; // 指し手
	bool winner; // 勝ったかどうか
	bool useLearning; // 学習に使うかどうか
	bool otherPVExist; // 棋譜の手と近い点数の手があったか。useLearning == true のときだけ有効な値が入る
};

class Learner {
public:
	void learn(Position& pos, std::istringstream& ssCmd) {
		eval_.initOptions(pos.GetSearcher()->options["Eval_Dir"], false);
		s64 gameNum;
		std::string recordFileName;
		std::string blackRecordFileName;
		std::string whiteRecordFileName;
		size_t threadNum;
		s64 updateMax;
		s64 updateMin;
		ssCmd >> recordFileName;
		ssCmd >> blackRecordFileName;
		ssCmd >> whiteRecordFileName;
		ssCmd >> gameNum;
		ssCmd >> threadNum;
		ssCmd >> minDepth_;
		ssCmd >> maxDepth_;
		ssCmd >> stepNum_;
		ssCmd >> gameNumForIteration_;
		ssCmd >> updateMax;
		ssCmd >> updateMin;
		ssCmd >> usePenalty_;
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
		updateMaxMask_ = (UINT64_C(1) << updateMax) - 1;
		updateMinMask_ = (UINT64_C(1) << updateMin) - 1;
		setUpdateMask(stepNum_);
		readBook(pos, recordFileName, blackRecordFileName, whiteRecordFileName, gameNum);
		// 既に 1 つのSearcher, Positionが立ち上がっているので、指定した数 - 1 の Searcher, Position を立ち上げる。
		threadNum = std::max<size_t>(0, threadNum - 1);
		std::vector<Searcher> searchers(threadNum);
		for (auto& s : searchers) {
			s.initOptions();
			setLearnOptions(s);
			positions_.push_back(Position(DefaultStartPositionSFEN, s.threads.mainThread(), s.thisptr));
			mts_.push_back(std::mt19937(std::chrono::system_clock::now().time_since_epoch().count()));
			// ここでデフォルトコンストラクタでpush_backすると、
			// 一時オブジェクトのParse2Dataがスタックに出来ることでプログラムが落ちるので、コピーコンストラクタにする。
			parse2Datum_.push_back(parse2Data_);
		}
		setLearnOptions(*pos.GetSearcher());
		mt_ = std::mt19937(std::chrono::system_clock::now().time_since_epoch().count());
		mt64_ = std::mt19937_64(std::chrono::system_clock::now().time_since_epoch().count());
		for (int i = 0; ; ++i) {
			std::cout << "iteration " << i << std::endl;
			std::cout << "parse1 start" << std::endl;
			learnParse1(pos);
			std::cout << "parse2 start" << std::endl;
			learnParse2(pos);
		}
	}
private:
	// 学習に使う棋譜から、手と手に対する補助的な情報を付けでデータ保持する。
	// 50000局程度に対して10秒程度で終わるからシングルコアで良い。
	void setLearnMoves(Position& pos, std::SetP<std::pair<Key, Move> >& dict, std::string& s0, std::string& s1,
					   const std::array<bool, ColorNum>& useTurnMove)
	{
		bookMovesDatum_.push_back(std::vector<BookMoveData>());
		BookMoveData bmdBase[ColorNum];
		bmdBase[Black].move = bmdBase[White].move = Move::moveNone();
		std::stringstream ss(s0);
		std::string elem;
		ss >> elem; // 対局番号
		ss >> elem; // 対局日
		bmdBase[Black].date = bmdBase[White].date = elem;
		ss >> elem; // 先手名
		bmdBase[Black].player = elem;
		ss >> elem; // 後手名
		bmdBase[White].player = elem;
		ss >> elem; // 引き分け勝ち負け
		bmdBase[Black].winner = (elem == "1");
		bmdBase[White].winner = (elem == "2");
		pos.SetP(DefaultStartPositionSFEN, pos.GetSearcher()->threads.mainThread());
		StateStackPtr setUpStates = StateStackPtr(new std::stack<StateInfo>());
		UsiOperation usiOperation;
		while (true) {
			const std::string moveStrCSA = s1.substr(0, 6);
			const Move move = usiOperation::csaToMove(pos, moveStrCSA);
			// 指し手の文字列のサイズが足りなかったり、反則手だったりすれば move.isNone() == true となるので、break する。
			if (move.isNone())
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
			pos.DoMove(move, setUpStates->top());
		}
	}
	void readBookBody(std::SetP<std::pair<Key, Move> >& dict, Position& pos, const std::string& record, const std::array<bool, ColorNum>& useTurnMove, const s64 gameNum)
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
		std::cout << "games existed: " << bookMovesDatum_.m_size() << std::endl;
	}
	void readBook(Position& pos,
				  const std::string& recordFileName,
				  const std::string& blackRecordFileName,
				  const std::string& whiteRecordFileName, const s64 gameNum)
	{
		std::SetP<std::pair<Key, Move> > dict;
		readBookBody(dict, pos,      recordFileName, {true , true }, gameNum);
		readBookBody(dict, pos, blackRecordFileName, {true , false}, gameNum);
		readBookBody(dict, pos, whiteRecordFileName, {false, true }, gameNum);
		gameNumForIteration_ = std::min(gameNumForIteration_, bookMovesDatum_.m_size());
	}
	void setLearnOptions(Searcher& s) {
		std::string options[] = {"name Threads value 1",
								 "name MultiPV value 1",
								 "name OwnBook value false",
								 "name Max_Random_Score_Diff value 0"};
		for (auto& str : options) {
			std::istringstream is(str);
			s.setOption(is);
		}
	}
	template <bool Dump> size_t lockingIndexIncrement() {
		std::unique_lock<Mutex> lock(mutex_);
		if (Dump) {
			if      (index_ % 500 == 0) std::cout << index_ << std::endl;
			else if (index_ % 100 == 0) std::cout << "o" << std::flush;
			else if (index_ %  10 == 0) std::cout << "." << std::flush;
		}
		return index_++;
	}
	void learnParse1Body(Position& pos, std::mt19937& mt) {
		std::uniform_int_distribution<Ply> dist(minDepth_, maxDepth_);
		pos.GetSearcher()->tt.Clear();
		for (size_t i = lockingIndexIncrement<true>(); i < gameNumForIteration_; i = lockingIndexIncrement<true>()) {
			StateStackPtr setUpStates = StateStackPtr(new std::stack<StateInfo>());
			pos.SetP(DefaultStartPositionSFEN, pos.GetSearcher()->threads.mainThread());
			auto& gameMoves = bookMovesDatum_[i];
			for (auto& bmd : gameMoves) {
				if (bmd.useLearning) {
					pos.GetSearcher()->alpha = -ScoreMaxEvaluate;
					pos.GetSearcher()->beta  =  ScoreMaxEvaluate;
					go(pos, dist(mt), bmd.move);
					const Score recordScore = pos.GetSearcher()->rootMoves[0].score_;
					++moveCount_;
					bmd.otherPVExist = false;
					bmd.pvBuffer.Clear();
					if (abs(recordScore) < ScoreMaxEvaluate) {
						int recordIsNth = 0; // 正解の手が何番目に良い手か。0から数える。
						auto& recordPv = pos.GetSearcher()->rootMoves[0].pv_;
						bmd.pvBuffer.insert(std::end(bmd.pvBuffer), std::begin(recordPv), std::end(recordPv));
						const auto recordPVSize = bmd.pvBuffer.m_size();
						for (MoveList<LegalAll> ml(pos); !ml.end(); ++ml) {
							if (ml.move() != bmd.move) {
								pos.GetSearcher()->alpha = recordScore - FVWindow;
								pos.GetSearcher()->beta  = recordScore + FVWindow;
								go(pos, dist(mt), ml.move());
								const Score score = pos.GetSearcher()->rootMoves[0].score_;
								if (pos.GetSearcher()->alpha < score && score < pos.GetSearcher()->beta) {
									auto& pv = pos.GetSearcher()->rootMoves[0].pv_;
									bmd.pvBuffer.insert(std::end(bmd.pvBuffer), std::begin(pv), std::end(pv));
								}
								if (recordScore < score)
									++recordIsNth;
							}
						}
						bmd.otherPVExist = (recordPVSize != bmd.pvBuffer.m_size());
						for (int i = recordIsNth; i < PredSize; ++i)
							++predictions_[i];
					}
				}
				setUpStates->push(StateInfo());
				pos.DoMove(bmd.move, setUpStates->top());
			}
		}
	}
	void learnParse1(Position& pos) {
		Time t = Time::CurrentTime();
		// 棋譜をシャッフルすることで、先頭 gameNum_ 個の学習に使うデータをランダムに選ぶ。
		std::shuffle(std::begin(bookMovesDatum_), std::end(bookMovesDatum_), mt_);
		std::cout << "shuffle elapsed: " << t.Elapsed() / 1000 << "[sec]" << std::endl;
		index_ = 0;
		moveCount_.store(0);
		for (auto& pred : predictions_)
			pred.store(0);
		std::vector<std::thread> threads(positions_.m_size());
		for (size_t i = 0; i < positions_.m_size(); ++i)
			threads[i] = std::thread([this, i] { learnParse1Body(positions_[i], mts_[i]); });
		learnParse1Body(pos, mt_);
		for (auto& thread : threads)
			thread.join();

		std::cout << "\nGames = " << bookMovesDatum_.m_size()
				  << "\nTotal Moves = " << moveCount_
				  << "\nPrediction = ";
		for (auto& pred : predictions_)
			std::cout << static_cast<double>(pred.load()*100) / moveCount_.load() << ", ";
		std::cout << std::endl;
		std::cout << "parse1 elapsed: " << t.Elapsed() / 1000 << "[sec]" << std::endl;
	}
	static constexpr double FVPenalty() { return (0.2/static_cast<double>(FVScale)); }
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
	template <bool UsePenalty>
	void updateEval(const std::string& dirName) {
		for (size_t i = 0; i < eval_.kpps_end_index(); ++i)
			updateFV<UsePenalty>(*eval_.GetKppOneArrayFirst(i), *parse2EvalBase_.GetKppOneArrayFirst(i));
		for (size_t i = 0; i < eval_.kkps_end_index(); ++i)
			updateFV<UsePenalty>(*eval_.GetKkpOneArrayFirst(i), *parse2EvalBase_.GetKkpOneArrayFirst(i));
		for (size_t i = 0; i < eval_.kks_end_index(); ++i)
			updateFV<UsePenalty>(*eval_.GetKkOneArrayFirst(i), *parse2EvalBase_.GetKkOneArrayFirst(i));

		// 学習しないパラメータがある時は、一旦 write() で学習しているパラメータだけ書きこんで、再度読み込む事で、
		// updateFV()で学習しないパラメータに入ったノイズを無くす。
		eval_.write(dirName);
		eval_.initOptions(dirName, false);
		g_evalTable.Clear();
	}
	double sigmoid(const double x) const {
		const double a = 7.0/static_cast<double>(FVWindow);
		const double clipx = std::max(static_cast<double>(-FVWindow), std::min(static_cast<double>(FVWindow), x));
		return 1.0 / (1.0 + exp(-a * clipx));
	}
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
	void setUpdateMask(const int step) {
		const int stepMax = stepNum_;
		const int max = count1s(updateMaxMask_);
		const int min = count1s(updateMinMask_);
		updateMask_ = max - (((max - min)*step+(stepMax>>1))/stepMax);
	}
	void learnParse2Body(Position& pos, Parse2Data& parse2Data) {
		parse2Data.Clear();
		SearchStack ss[2];
		for (size_t i = lockingIndexIncrement<false>(); i < gameNumForIteration_; i = lockingIndexIncrement<false>()) {
			StateStackPtr setUpStates = StateStackPtr(new std::stack<StateInfo>());
			pos.SetP(DefaultStartPositionSFEN, pos.GetSearcher()->threads.mainThread());
			auto& gameMoves = bookMovesDatum_[i];
			for (auto& bmd : gameMoves) {
				PRINT_PV(pos.Print());
				if (bmd.useLearning && bmd.otherPVExist) {
					const Color rootColor = pos.GetTurn();
					int recordPVIndex = 0;
					PRINT_PV(std::cout << "recordpv: ");
					for (; !bmd.pvBuffer[recordPVIndex].isNone(); ++recordPVIndex) {
						PRINT_PV(std::cout << bmd.pvBuffer[recordPVIndex].toCSA());
						setUpStates->push(StateInfo());
						pos.DoMove(bmd.pvBuffer[recordPVIndex], setUpStates->top());
					}
					// evaluate() の差分計算を無効化する。
					ss[0].staticEvalRaw.GetP[0][0] = ss[1].staticEvalRaw.GetP[0][0] = ScoreNotEvaluated;
					const Score recordScore = (rootColor == pos.GetTurn() ? evaluate(pos, ss+1) : -evaluate(pos, ss+1));
					PRINT_PV(std::cout << ", score: " << recordScore << std::endl);
					for (int jj = recordPVIndex - 1; 0 <= jj; --jj) {
						pos.UndoMove(bmd.pvBuffer[jj]);
					}

					std::array<double, 2> sum_dT = {{0.0, 0.0}};
					for (int otherPVIndex = recordPVIndex + 1; otherPVIndex < static_cast<int>(bmd.pvBuffer.m_size()); ++otherPVIndex) {
						PRINT_PV(std::cout << "otherpv : ");
						for (; !bmd.pvBuffer[otherPVIndex].isNone(); ++otherPVIndex) {
							PRINT_PV(std::cout << bmd.pvBuffer[otherPVIndex].toCSA());
							setUpStates->push(StateInfo());
							pos.DoMove(bmd.pvBuffer[otherPVIndex], setUpStates->top());
						}
						ss[0].staticEvalRaw.GetP[0][0] = ss[1].staticEvalRaw.GetP[0][0] = ScoreNotEvaluated;
						const Score score = (rootColor == pos.GetTurn() ? evaluate(pos, ss+1) : -evaluate(pos, ss+1));
						const auto diff = score - recordScore;
						const double dsig = dsigmoid(diff);
						std::array<double, 2> dT = {{(rootColor == Black ? dsig : -dsig), dsig}};
						PRINT_PV(std::cout << ", score: " << score << ", dT: " << dT[0] << std::endl);
						sum_dT += dT;
						dT[0] = -dT[0];
						dT[1] = (pos.GetTurn() == rootColor ? -dT[1] : dT[1]);
						parse2Data.params.incParam(pos, dT);
						for (int jj = otherPVIndex - 1; !bmd.pvBuffer[jj].isNone(); --jj) {
							pos.UndoMove(bmd.pvBuffer[jj]);
						}
					}

					for (int jj = 0; jj < recordPVIndex; ++jj) {
						setUpStates->push(StateInfo());
						pos.DoMove(bmd.pvBuffer[jj], setUpStates->top());
					}
					sum_dT[1] = (pos.GetTurn() == rootColor ? sum_dT[1] : -sum_dT[1]);
					parse2Data.params.incParam(pos, sum_dT);
					for (int jj = recordPVIndex - 1; 0 <= jj; --jj) {
						pos.UndoMove(bmd.pvBuffer[jj]);
					}
				}
				setUpStates->push(StateInfo());
				pos.DoMove(bmd.move, setUpStates->top());
			}
		}
	}
	void learnParse2(Position& pos) {
		Time t;
		for (int step = 1; step <= stepNum_; ++step) {
			t.Restart();
			std::cout << "step " << step << "/" << stepNum_ << " " << std::flush;
			index_ = 0;
			std::vector<std::thread> threads(positions_.m_size());
			for (size_t i = 0; i < positions_.m_size(); ++i)
				threads[i] = std::thread([this, i] { learnParse2Body(positions_[i], parse2Datum_[i]); });
			learnParse2Body(pos, parse2Data_);
			for (auto& thread : threads)
				thread.join();

			for (auto& parse2 : parse2Datum_) {
				parse2Data_.params += parse2.params;
			}
			parse2EvalBase_.Clear();
			lowerDimension(parse2EvalBase_, parse2Data_.params);
			setUpdateMask(step);
			std::cout << "update eval ... " << std::flush;
			if (usePenalty_) updateEval<true >(pos.GetSearcher()->options["Eval_Dir"]);
			else             updateEval<false>(pos.GetSearcher()->options["Eval_Dir"]);
			std::cout << "done" << std::endl;
			std::cout << "parse2 1 step elapsed: " << t.Elapsed() / 1000 << "[sec]" << std::endl;
			Print();
		}
	}
	void Print() {
		for (Rank r = Rank9; r < RankNum; ++r) {
			for (File f = FileA; FileI <= f; --f) {
				const Square sq = FromFileRank(f, r);
				printf("%5d", EvalStorage::KPP[B2][f_gold + C2][f_gold + sq][0]);
			}
			printf("\n");
		}
		printf("\n");
		fflush(stdout);
	}

	static const int PredSize = 8;
	static const Score FVWindow = static_cast<Score>(256);

	Mutex mutex_;
	size_t index_;
	Ply minDepth_;
	Ply maxDepth_;
	bool usePenalty_;
	std::mt19937 mt_;
	std::mt19937_64 mt64_;
	std::vector<std::mt19937> mts_;
	std::vector<Position> positions_;
	std::vector<std::vector<BookMoveData> > bookMovesDatum_;
	std::atomic<s64> moveCount_;
	std::atomic<s64> predictions_[PredSize];
	Parse2Data parse2Data_;
	std::vector<Parse2Data> parse2Datum_;
	EvaluaterBase<std::array<std::atomic<float>, 2>,
				  std::array<std::atomic<float>, 2>,
				  std::array<std::atomic<float>, 2> > parse2EvalBase_;
	EvalStorage eval_;
	int stepNum_;
	size_t gameNumForIteration_;
	u64 updateMaxMask_;
	u64 updateMinMask_;
	u64 updateMask_;
};

#endif

