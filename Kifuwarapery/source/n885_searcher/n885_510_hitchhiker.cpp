#include <iostream>
#include "../../header/n119_score___/n119_200_pieceScore.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_220_queenAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"

#include "../../header/n220_position/n220_100_repetitionType.hpp"
#include "../../header/n220_position/n220_640_utilAttack.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_665_utilMoveStack.hpp"
#include "../../header/n220_position/n220_670_makePromoteMove.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"

#include "../../header/n223_move____/n223_040_nodeType.hpp"
#include "../../header/n223_move____/n223_300_moveAndScoreIndex.hpp"

#include "../../header/n350_pieceTyp/n350_500_ptPrograms.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../../header/n520_evaluate/n520_700_evaluation09.hpp"
#include "../../header/n560_timeMng_/n560_500_timeManager.hpp"
#include "../../header/n600_book____/n600_500_book.hpp"

#include "../../header/n640_searcher/n640_440_splitedNode.hpp"
#include "../../header/n640_searcher/n640_500_reductions.hpp"
#include "../../header/n640_searcher/n640_510_futilityMargins.hpp"
#include "../../header/n640_searcher/n640_520_futilityMoveCounts.hpp"
#include "../../header/n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../../header/n680_egOption/n680_300_engineOptionSetup.hpp"
#include "../../header/n760_thread__/n760_400_herosPub.hpp"

#include "../../header/n800_learn___/n800_100_stopwatch.hpp"
#include "../../header/n883_nodeType/n883_070_nodetypeAbstract.hpp"

#include "../../header/n885_searcher/n885_040_rucksack.hpp"
#include "../../header/n885_searcher/n885_480_hitchhikerNyugyoku.hpp"
#include "../../header/n885_searcher/n885_510_hitchhiker.hpp"
#include "../../header/n885_searcher/n885_600_iterativeDeepeningLoop.hpp"//FIXME:

#include "../../header/n886_repeType/n886_100_rtNot.hpp"
#include "../../header/n886_repeType/n886_110_rtDraw.hpp"
#include "../../header/n886_repeType/n886_120_rtWin.hpp"
#include "../../header/n886_repeType/n886_130_rtLose.hpp"
#include "../../header/n886_repeType/n886_140_rtSuperior.hpp"
#include "../../header/n886_repeType/n886_150_rtInferior.hpp"
#include "../../header/n886_repeType/n886_500_rtArray.hpp"

#include "../../header/n887_nodeType/n887_500_nodetypePrograms.hpp"//FIXME:
//class IterativeDeepeningLoop;
//static inline void IterativeDeepeningLoop::Execute(Rucksack& rucksack, Position& pos);

using namespace std;


extern const InFrontMaskBb g_inFrontMaskBb;
extern NodetypeAbstract* g_NODETYPE_PROGRAMS[];
extern RepetitionTypeArray g_repetitionTypeArray;



void Hitchhiker::Think(Rucksack& rucksack) {

	Position& pos = rucksack.m_rootPosition;
	rucksack.m_timeManager.Init(rucksack.m_limits, pos.GetGamePly(), pos.GetTurn(), &rucksack);

	//────────────────────────────────────────────────────────────────────────────────
	// 入玉勝ちかを調べるぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
#if !defined(LEARN)
	if (Hitchhiker::IsNyugyokuWin(pos)) {
		//────────────────────────────────────────────────────────────────────────────────
		// 探索☆？（＾ｑ＾）
		//────────────────────────────────────────────────────────────────────────────────
		HitchhikerNyugyoku::Travel_885_480(rucksack,pos);
		return;
	}
#endif

	//────────────────────────────────────────────────────────────────────────────────
	// 入玉勝ちじゃなかったぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	static Book book;
	std::uniform_int_distribution<int> dist(
		rucksack.m_engineOptions["Min_Book_Ply"], rucksack.m_engineOptions["Max_Book_Ply"]);

	const Ply book_ply = dist(g_randomTimeSeed);

	pos.SetNodesSearched(0);

#if defined LEARN
	rucksack.m_ownerHerosPub[0]->m_searching = true;
#else
	rucksack.m_tt.SetSize(rucksack.m_engineOptions["USI_Hash"]); // operator int() 呼び出し。

	SYNCCOUT << "info string book_ply " << book_ply << SYNCENDL;
	if (
		// 定跡が使える手数のとき☆（＾ｑ＾）？
		rucksack.m_engineOptions["OwnBook"] &&
		pos.GetGamePly() <= book_ply
	) {
		const MoveAndScoreIndex bookMoveScore = book.GetProbe(
			pos, rucksack.m_engineOptions["Book_File"], rucksack.m_engineOptions["Best_Book_Move"]);

		if (
			!bookMoveScore.m_move.IsNone()
			&&
			std::find(
				rucksack.m_rootMoves.begin(),
				rucksack.m_rootMoves.end(),
				bookMoveScore.m_move
			) != rucksack.m_rootMoves.end()
		){
			std::swap(rucksack.m_rootMoves[0], *std::find(rucksack.m_rootMoves.begin(),
				rucksack.m_rootMoves.end(),
				bookMoveScore.m_move));
			SYNCCOUT << "info"
				<< " score " << rucksack.scoreToUSI(bookMoveScore.m_scoreIndex)
				<< " pv " << bookMoveScore.m_move.ToUSI()
				<< SYNCENDL;

			goto finalize;
		}
	}

	// 全スレッドの初期化か何か☆？
	rucksack.m_ownerHerosPub.WakeUp(&rucksack);

	// 下級戦士の寿命（ミリ秒）を設定するぜ☆
	rucksack.m_ownerHerosPub.GetCurrWarrior()->m_lifetimeMilliseconds =
		(rucksack.m_limits.IsUseTimeManagement() ?
			std::min(100, std::max(rucksack.m_timeManager.GetAvailableTime() / 16, rucksack.TimerResolution)) :
			rucksack.m_limits.m_nodes ?
			2 * rucksack.TimerResolution :
			100
			);

	rucksack.m_ownerHerosPub.GetCurrWarrior()->NotifyOne();

#if defined INANIWA_SHIFT
	detectInaniwa(GetPos);
#endif
#if defined BISHOP_IN_DANGER
	detectBishopInDanger(GetPos);
#endif
#endif
	//────────────────────────────────────────────────────────────────────────────────
	// 反復深化探索を始めるぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	IterativeDeepeningLoop::Execute885_500(rucksack, pos);//ExecuteIterativeDeepeningLoop(pos);

#if defined LEARN
#else
	rucksack.m_ownerHerosPub.GetCurrWarrior()->m_lifetimeMilliseconds = 0; // timer を止める。
	rucksack.m_ownerHerosPub.Sleep();

finalize:

	SYNCCOUT << "info nodes " << pos.GetNodesSearched()
		<< " time " << rucksack.m_stopwatch.GetElapsed() << SYNCENDL;

	if (!rucksack.m_signals.m_stop && (rucksack.m_limits.m_ponder || rucksack.m_limits.m_infinite)) {
		rucksack.m_signals.m_stopOnPonderHit = true;
		pos.GetThisThread()->WaitFor(rucksack.m_signals.m_stop);
	}

	if (rucksack.m_rootMoves[0].m_pv_[0].IsNone()) {
		SYNCCOUT << "bestmove resign" << SYNCENDL;
	}
	else {
		SYNCCOUT << "bestmove " << rucksack.m_rootMoves[0].m_pv_[0].ToUSI()
			<< " ponder " << rucksack.m_rootMoves[0].m_pv_[1].ToUSI()
			<< SYNCENDL;
	}
#endif
}



// 入玉勝ちかどうかを判定
bool Hitchhiker::IsNyugyokuWin(const Position& pos) {
	// CSA ルールでは、一 から 六 の条件を全て満たすとき、入玉勝ち宣言が出来る。

	// 一 宣言側の手番である。

	// この関数を呼び出すのは自分の手番のみとする。ponder では呼び出さない。

	const Color us = pos.GetTurn();

	// 敵陣のマスク
	const Bitboard opponentsField = (us == Black ? g_inFrontMaskBb.GetInFrontMask(Black, Rank6) : g_inFrontMaskBb.GetInFrontMask(White, Rank4));

	// 二 宣言側の玉が敵陣三段目以内に入っている。
	if (!pos.GetBbOf20(N08_King, us).AndIsNot0(opponentsField))
	{
		return false;
	}

	// 三 宣言側が、大駒5点小駒1点で計算して
	//     先手の場合28点以上の持点がある。
	//     後手の場合27点以上の持点がある。
	//     点数の対象となるのは、宣言側の持駒と敵陣三段目以内に存在する玉を除く宣言側の駒のみである。
	const Bitboard bigBB = pos.GetBbOf(N06_Rook, N14_Dragon, N05_Bishop, N13_Horse) & opponentsField & pos.GetBbOf10(us);

	const Bitboard smallBB = (pos.GetBbOf(N01_Pawn, N02_Lance, N03_Knight, N04_Silver) |
		pos.GetGoldsBB()) & opponentsField & pos.GetBbOf10(us);

	const Hand hand = pos.GetHand(us);

	const int val = (bigBB.PopCount() + hand.NumOf<HRook>() + hand.NumOf<HBishop>()) * 5
		+ smallBB.PopCount()
		+ hand.NumOf<HPawn>() + hand.NumOf<HLance>() + hand.NumOf<HKnight>()
		+ hand.NumOf<HSilver>() + hand.NumOf<HGold>();

#if defined LAW_24
	if (val < 31)
	{
		return false;
	}
#else
	if (val < (us == Black ? 28 : 27))
	{
		return false;
	}
#endif

	// 四 宣言側の敵陣三段目以内の駒は、玉を除いて10枚以上存在する。

	// 玉は敵陣にいるので、自駒が敵陣に11枚以上あればよい。
	if ((pos.GetBbOf10(us) & opponentsField).PopCount() < 11)
	{
		return false;
	}

	// 五 宣言側の玉に王手がかかっていない。
	if (pos.InCheck())
	{
		return false;
	}

	// 六 宣言側の持ち時間が残っている。

	// 持ち時間が無ければ既に負けていて　ここにはこないので、何もチェックしない（＾ｑ＾）

	return true;
}
