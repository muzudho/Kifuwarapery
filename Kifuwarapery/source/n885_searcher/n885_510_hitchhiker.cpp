#include <iostream>
#include "../../header/n119_score___/n119_200_pieceScore.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_220_queenAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n800_learn___/n800_100_stopwatch.hpp"
#include "../../header/n886_repeType/n886_100_rtNot.hpp"
#include "../../header/n886_repeType/n886_110_rtDraw.hpp"
#include "../../header/n886_repeType/n886_120_rtWin.hpp"
#include "../../header/n886_repeType/n886_130_rtLose.hpp"
#include "../../header/n886_repeType/n886_140_rtSuperior.hpp"
#include "../../header/n886_repeType/n886_150_rtInferior.hpp"
#include "../../header/n886_repeType/n886_500_rtArray.hpp"
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
#include "../../header/n883_nodeType/n883_070_nodetypeAbstract.hpp"

#include "../../header/n885_searcher/n885_040_rucksack.hpp"
#include "../../header/n885_searcher/n885_480_hitchhikerNyugyoku.hpp"
#include "../../header/n885_searcher/n885_600_iterativeDeepeningLoop.hpp"//FIXME:
#include "../../header/n887_nodeType/n887_500_nodetypePrograms.hpp"//FIXME:
//class IterativeDeepeningLoop;
//static inline void IterativeDeepeningLoop::Execute(Rucksack& rucksack, Position& pos);
#include "../../header/n885_searcher/n885_510_hitchhiker.hpp"

using namespace std;


extern const InFrontMaskBb g_inFrontMaskBb;
extern NodetypeAbstract* g_NODETYPE_PROGRAMS[];
extern RepetitionTypeArray g_repetitionTypeArray;




ScoreIndex Hitchhiker::Travel_885_510(
	Rucksack& rucksack,
	NodeType NT,
	Position& pos,
	Flashlight* pFlashlight,//サーチスタック
	ScoreIndex alpha,
	ScoreIndex beta,
	const Depth depth,
	const bool cutNode
	) {

	/*
	g_NODETYPE_PROGRAMS[NT]->GoToTheAdventure(
		rucksack,
		NT,
		pos,
		pFlashlight,//サーチスタック
		alpha,
		beta,
		depth,
		cutNode
		);
		*/

	NodetypeAbstract* nodetypeProgram = g_NODETYPE_PROGRAMS[NT];


	assert(-ScoreInfinite <= alpha && alpha < beta && beta <= ScoreInfinite);
	nodetypeProgram->AssertBeforeStep1(
		alpha,
		beta
		);
	assert(Depth0 < depth);

	// 途中で goto を使用している為、先に全部の変数を定義しておいた方が安全。
	Move movesSearched[64];
	StateInfo st;
	const TTEntry* pTtEntry = nullptr;//(^q^)トランスポジション・テーブル・エントリー☆？
	SplitedNode* pSplitedNode = nullptr;//(^q^)
	Key posKey;
	Move ttMove;
	Move move;
	Move excludedMove;
	Move bestMove;
	Move threatMove;
	Depth newDepth;
	Depth extension;
	ScoreIndex bestScore;
	ScoreIndex score;
	ScoreIndex ttScore;
	ScoreIndex eval;
	bool inCheck;
	bool givesCheck;
	bool isPVMove;
	bool singularExtensionNode;
	bool captureOrPawnPromotion;
	bool dangerous;
	bool doFullDepthSearch;
	int moveCount;
	int playedMoveCount;

	// step1
	// initialize node
	Military* pThisThread = pos.GetThisThread();
	moveCount = playedMoveCount = 0;
	inCheck = pos.InCheck();

	bool isGotoSplitPointStart = false;
	nodetypeProgram->DoStep1a(
		isGotoSplitPointStart,
		moveCount,
		playedMoveCount,
		inCheck,
		pos,
		&pSplitedNode,
		&pFlashlight,
		bestMove,
		threatMove,
		bestScore,
		ttMove,
		excludedMove,
		ttScore
		);
	if (isGotoSplitPointStart)
	{
		goto split_point_start;
	}

	nodetypeProgram->DoStep1b(
		bestScore,
		&pFlashlight,
		threatMove,
		bestMove
	);
	nodetypeProgram->DoStep1c(
		&pThisThread,
		pFlashlight
		);

	bool isReturnWithScore = false;
	ScoreIndex returnScore = ScoreIndex::ScoreNone;

	// step2
	nodetypeProgram->DoStep2(
		isReturnWithScore,
		returnScore,
		pos,
		rucksack,
		&pFlashlight
	);

	if (isReturnWithScore)
	{
		return returnScore;
	}

	// step3
	nodetypeProgram->DoStep3(
		isReturnWithScore,
		returnScore,
		&pFlashlight,
		alpha,
		beta
		);
	if (isReturnWithScore)
	{
		return returnScore;
	}

	pos.SetNodesSearched(pos.GetNodesSearched() + 1);

	// step4
	nodetypeProgram->DoStep4(
		excludedMove,
		&pFlashlight,
		posKey,
		pos,
		&pTtEntry,//セットされる☆
		rucksack,
		ttScore
		);
	nodetypeProgram->DoStep4x(
		ttMove,
		rucksack,
		pTtEntry,
		pos
		);
	nodetypeProgram->DoStep4y(
		isReturnWithScore,
		returnScore,
		rucksack,
		pTtEntry,
		depth,
		ttScore,
		beta,
		&pFlashlight,
		ttMove
		);
	if (isReturnWithScore)
	{
		return returnScore;
	}
	nodetypeProgram->DoStep4z(
		isReturnWithScore,
		returnScore,
		rucksack,
		inCheck,
		move,
		pos,
		&pFlashlight,
		bestScore,
		posKey,
		depth,
		bestMove
		);
	if (isReturnWithScore)
	{
		return returnScore;
	}

	// step5
	bool isGotoIidStart = false;
	nodetypeProgram->DoStep5(
		isGotoIidStart,
		rucksack,
		eval,
		&pFlashlight,
		pos,
		inCheck,
		pTtEntry,
		ttScore,
		posKey,
		move
		);
	if (isGotoIidStart) {
		goto iid_start;
	}

	// step6
	nodetypeProgram->DoStep6_NonPV(
		isReturnWithScore,
		returnScore,
		rucksack,
		depth,
		eval,
		beta,
		ttMove,
		pos,
		&pFlashlight
		);
	if (isReturnWithScore)
	{
		return returnScore;
	}

	// step7
	nodetypeProgram->DoStep7(
		isReturnWithScore,
		returnScore,
		&pFlashlight,
		depth,
		beta,
		eval
		);

	// step8
	nodetypeProgram->DoStep8_NonPV(
		isReturnWithScore,
		returnScore,
		rucksack,
		&pFlashlight,
		depth,
		beta,
		eval,
		pos,
		st,
		alpha,
		cutNode,
		threatMove
	);
	if (isReturnWithScore) {
		return returnScore;
	}

	// step9
	nodetypeProgram->DoStep9(
		isReturnWithScore,
		rucksack,
		depth,
		&pFlashlight,
		beta,
		move,
		pos,
		ttMove,
		st,
		score,
		cutNode
		);
	if (isReturnWithScore) {
		return score;
	}

	// 内側の反復深化探索☆？（＾ｑ＾）
iid_start:
	// step10
	nodetypeProgram->DoStep10(
		depth,
		ttMove,
		inCheck,
		beta,
		&pFlashlight,
		rucksack,
		pos,
		alpha,
		&pTtEntry,//セットされるぜ☆
		posKey
		);

split_point_start:
	NextmoveEvent mp(
		pos,
		ttMove,
		depth,
		rucksack.m_history,
		pFlashlight,
		nodetypeProgram->GetBetaAtStep11( beta)//PVノードか、そうでないかで初期値を変えるぜ☆（＾ｑ＾）
	);
	const CheckInfo ci(pos);

	nodetypeProgram->DoStep11A_BeforeLoop_SplitPointStart(
		ttMove,
		depth,
		score,
		bestScore,
		singularExtensionNode,
		excludedMove,
		pTtEntry//pv,nonPv の２つで、nullptrはダメ☆
		);

	// step11
	// Loop through moves
	while (
		!(
			// スプリット・ポイントかどうかで、取ってくる指し手が変わる☆
			move = nodetypeProgram->GetMoveAtStep11(mp)
		).IsNone()
	) {

		bool isContinue = false;
		nodetypeProgram->DoStep11Ba_LoopHeader(
			isContinue,
			move,
			excludedMove
			);
		if (isContinue)
		{
			continue;
		}

		nodetypeProgram->DoStep11Bb_LoopHeader(
			isContinue,
			pos,
			move,
			ci,
			moveCount,
			&pSplitedNode
			);
		if (isContinue)
		{
			continue;
		}

		nodetypeProgram->DoStep11Bb_LoopHeader(
			isContinue,
			rucksack,
			move
			);
		if (isContinue)
		{
			continue;
		}

		nodetypeProgram->DoStep11Bc_LoopHeader(
			rucksack,
			moveCount
			);

		nodetypeProgram->DoStep11B_LoopHeader(
			extension,
			captureOrPawnPromotion,
			move,
			givesCheck,
			ci,
			pos,
			dangerous
			);
		if (isContinue)
		{
			continue;
		}

		// step12
		nodetypeProgram->DoStep12(
			rucksack,
			givesCheck,
			pos,
			move,
			extension,
			singularExtensionNode,
			ttMove,
			ttScore,
			ci,
			depth,
			&pFlashlight,
			score,
			cutNode,
			beta,
			newDepth
			);

		// step13
		nodetypeProgram->DoStep13a(
			isContinue,
			rucksack,
			captureOrPawnPromotion,
			inCheck,
			dangerous,
			bestScore,
			move,
			ttMove,
			depth,
			moveCount,
			threatMove,
			pos,
			&pSplitedNode,
			newDepth,
			&pFlashlight,
			beta
			);
		nodetypeProgram->DoStep13b(
			pos,
			move,
			ci,
			moveCount,
			isContinue
			);
		nodetypeProgram->DoStep13c(
			isContinue,
			rucksack,
			captureOrPawnPromotion,
			inCheck,
			dangerous,
			bestScore,
			move,
			ttMove,
			depth,
			moveCount,
			threatMove,
			pos,
			&pSplitedNode,
			newDepth,
			&pFlashlight,
			beta,
			ci,
			isPVMove,
			playedMoveCount,
			movesSearched
			);
		if (isContinue)
		{
			continue;
		}
		nodetypeProgram->DoStep13d(
			captureOrPawnPromotion,
			playedMoveCount,
			movesSearched,
			move
			);

		// step14
		nodetypeProgram->DoStep14(
			pos,
			move,
			st,
			ci,
			givesCheck,
			&pFlashlight
			);

		// step15
		nodetypeProgram->DoStep15(
			rucksack,
			depth,
			isPVMove,
			captureOrPawnPromotion,
			move,
			ttMove,
			&pFlashlight,
			moveCount,
			cutNode,
			newDepth,
			alpha,
			&pSplitedNode,
			score,
			pos,
			doFullDepthSearch
			);

		// step16
		nodetypeProgram->DoStep16a(
			doFullDepthSearch,
			alpha,
			&pSplitedNode
			);
		nodetypeProgram->DoStep16b_NonPVAtukai(
			rucksack,
			doFullDepthSearch,
			score,
			newDepth,
			givesCheck,
			pos,
			&pFlashlight,
			alpha,
			cutNode
			);
		nodetypeProgram->DoStep16c(
			rucksack,
			isPVMove,
			alpha,
			score,
			beta,
			newDepth,
			givesCheck,
			pos,
			&pFlashlight
			);

		// step17
		nodetypeProgram->DoStep17(
			pos,
			move
			);

		assert(-ScoreInfinite < score && score < ScoreInfinite);

		// step18
		nodetypeProgram->DoStep18a(
			&pSplitedNode,
			bestScore,
			alpha
			);

		if (rucksack.m_signals.m_stop || pThisThread->CutoffOccurred()) {
			return score;
		}

		nodetypeProgram->DoStep18b(
			rucksack,
			move,
			isPVMove,
			alpha,
			score,
			pos
			);
		bool isBreak = false;
		nodetypeProgram->DoStep18c(
			isBreak,
			rucksack,
			move,
			isPVMove,
			alpha,
			score,
			pos,
			bestScore,
			&pSplitedNode,
			bestMove,
			beta
			);
		if (isBreak) {
			break;
		}

		// step19
		nodetypeProgram->DoStep19(
			isBreak,
			rucksack,
			depth,
			&pThisThread,
			bestScore,
			beta,
			pos,
			&pFlashlight,
			alpha,
			bestMove,
			threatMove,
			moveCount,
			mp,
			NT,
			cutNode
			);
		if (isBreak) {
			break;
		}
	}

	if (nodetypeProgram->GetReturnBeforeStep20()) {
		return bestScore;
	}

	// step20
	nodetypeProgram->DoStep20(
		moveCount,
		excludedMove,
		rucksack,
		alpha,
		&pFlashlight,
		bestScore,
		playedMoveCount,
		beta,
		posKey,
		depth,
		bestMove,
		inCheck,
		pos,
		movesSearched
		);

	return bestScore;
}





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
