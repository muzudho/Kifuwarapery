#pragma once

#include "../../header/n080_common__/n080_105_time.hpp"
#include "../../header/n200_score___/n200_200_pieceScore.hpp"
#include "../../header/n240_position/n240_150_move.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n240_position/n240_400_MoveScore.hpp"
#include "../../header/n270_timeMng_/n270_100_timeManager.hpp"
#include "../../header/n320_operate_/n320_125_searchStack.hpp"
#include "../../header/n320_operate_/n320_128_signalsType.hpp"
#include "../../header/n360_egOption/n360_230_engineOptionable.hpp"
#include "../../header/n360_egOption/n360_240_engineOptionsMap.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"

using namespace std;


class Position;
struct SplitPoint;


enum InaniwaFlag {
	NotInaniwa,
	InaniwaIsBlack,
	InaniwaIsWhite,
	InaniwaFlagNum
};

enum BishopInDangerFlag {
	NotBishopInDanger,
	BlackBishopInDangerIn28,
	WhiteBishopInDangerIn28,
	BlackBishopInDangerIn78,
	WhiteBishopInDangerIn78,
	BishopInDangerFlagNum
};

class RootMove {
public:
	RootMove() {}
	explicit RootMove(const Move m) : score_(-ScoreInfinite), prevScore_(-ScoreInfinite) {
		pv_.push_back(m);
		pv_.push_back(Move::moveNone());
	}
	explicit RootMove(const MoveScore m) : score_(m.score), prevScore_(-ScoreInfinite) {
		pv_.push_back(m.move);
		pv_.push_back(Move::moveNone());
	}

	bool operator < (const RootMove& m) const {
		return score_ < m.score_;
	}
	bool operator == (const Move& m) const {
		return pv_[0] == m;
	}

	void extractPvFromTT(Position& pos);
	void insertPvInTT(Position& pos);

public:
	Score score_;
	Score prevScore_;
	std::vector<Move> pv_;
};

template <bool Gain>
class Stats {
public:
	static const Score MaxScore = static_cast<Score>(2000);

	void clear() { memset(table_, 0, sizeof(table_)); }
	Score value(const bool isDrop, const Piece pc, const Square to) const {
		assert(0 < pc && pc < PieceNone);
		assert(isInSquare(to));
		return table_[isDrop][pc][to];
	}
	void update(const bool isDrop, const Piece pc, const Square to, const Score s) {
		if (Gain) {
			table_[isDrop][pc][to] = std::max(s, value(isDrop, pc, to) - 1);
		}
		else if (abs(value(isDrop, pc, to) + s) < MaxScore) {
			table_[isDrop][pc][to] += s;
		}
	}

private:
	// [isDrop][piece][square] とする。
	Score table_[2][PieceNone][SquareNum];
};

using History = Stats<false>;
using Gains   = Stats<true>;

class TranspositionTable;

// 検索のための構造体？
class Searcher {
public:
	// static メンバ関数からだとthis呼べないので代わりに thisptr を使う。
	// static じゃないときは this を入れることにする。
	STATIC Searcher*				thisptr;
	STATIC volatile SignalsType		signals;
	STATIC LimitsType				limits;
	STATIC std::vector<Move>		searchMoves;
	STATIC Time						searchTimer;
	STATIC StateStackPtr			setUpStates;
	STATIC std::vector<RootMove>	rootMoves;

#if defined LEARN
	STATIC Score					alpha;
	STATIC Score					beta;
#endif

	STATIC size_t					pvSize;
	STATIC size_t					pvIdx;
	STATIC TimeManager				timeManager;
	STATIC Ply						bestMoveChanges;
	STATIC History					history;
	STATIC Gains					gains;
	STATIC TranspositionTable		tt;

#if defined INANIWA_SHIFT
	STATIC InaniwaFlag				inaniwaFlag;
#endif
#if defined BISHOP_IN_DANGER
	STATIC BishopInDangerFlag		bishopInDangerFlag;
#endif
	STATIC Position					rootPosition;
	STATIC ThreadPool				threads;
	STATIC EngineOptionsMap			options;

	STATIC void						init();
	STATIC void						idLoop(Position& pos);
	STATIC std::string				pvInfoToUSI(Position& pos, const Ply depth, const Score alpha, const Score beta);
	template <NodeType NT, bool INCHECK>
	STATIC Score					qsearch(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth);
#if defined INANIWA_SHIFT
	STATIC void						detectInaniwa(const Position& pos);
#endif
#if defined BISHOP_IN_DANGER
	STATIC void						detectBishopInDanger(const Position& pos);
#endif
	template <NodeType NT>
	STATIC Score					search(Position& pos, SearchStack* ss, Score alpha, Score beta, const Depth depth, const bool cutNode);
	STATIC void						think();
	STATIC void						checkTime();

	//STATIC void						doUSICommandLoop(int argc, char* argv[]);
	STATIC void						setOption(std::istringstream& ssCmd);
};

void initSearchTable();

