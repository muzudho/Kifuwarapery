#include <iostream>
#include <algorithm>
#include "../../header/n119_score___/n119_200_pieceScore.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n160_board___/n160_220_queenAttackBb.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"

#include "../../header/n220_position/n220_100_repetitionType.hpp"
#include "../../header/n220_position/n220_640_utilAttack.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n220_position/n220_665_utilMoveStack.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"

#include "../../header/n223_move____/n223_040_nodeType.hpp"
#include "../../header/n223_move____/n223_300_moveAndScoreIndex.hpp"
#include "../../header/n223_move____/n223_500_flashlight.hpp"

#include "../../header/n350_pieceTyp/n350_030_makePromoteMove.hpp"
#include "../../header/n350_pieceTyp/n350_500_ptPrograms.hpp"
#include "../../header/n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../../header/n520_evaluate/n520_700_evaluation09.hpp"
#include "../../header/n560_timeMng_/n560_500_timeManager.hpp"
#include "../../header/n600_book____/n600_500_book.hpp"

#include "../../header/n640_searcher/n640_440_splitedNode.hpp" // Searcherと持ち合い
#include "../../header/n640_searcher/n640_500_reductions.hpp"
#include "../../header/n640_searcher/n640_510_futilityMargins.hpp"
#include "../../header/n640_searcher/n640_520_futilityMoveCounts.hpp"
#include "../../header/n680_egOption/n680_240_engineOptionsMap.hpp"
#include "../../header/n680_egOption/n680_300_engineOptionSetup.hpp"
#include "../../header/n760_thread__/n760_400_herosPub.hpp"

#include "../../header/n800_learn___/n800_100_stopwatch.hpp"
#include "../../header/n883_nodeType/n883_070_nodetypeAbstract.hpp"

#include "../../header/n885_searcher/n885_040_rucksack.hpp"
#include "../../header/n885_searcher/n885_310_hitchhikerQsearchAbstract.hpp"
#include "../../header/n885_searcher/n885_340_hitchhikerQsearchPrograms.hpp"
#include "../../header/n885_searcher/n885_480_hitchhikerNyugyoku.hpp"
#include "../../header/n885_searcher/n885_510_hitchhiker.hpp"
#include "../../header/n885_searcher/n885_600_iterativeDeepeningLoop.hpp"

#include "../../header/n886_repeType/n886_100_rtNot.hpp"
#include "../../header/n886_repeType/n886_110_rtDraw.hpp"
#include "../../header/n886_repeType/n886_120_rtWin.hpp"
#include "../../header/n886_repeType/n886_130_rtLose.hpp"
#include "../../header/n886_repeType/n886_140_rtSuperior.hpp"
#include "../../header/n886_repeType/n886_150_rtInferior.hpp"
#include "../../header/n886_repeType/n886_500_rtArray.hpp"

#include "../../header/n887_nodeType/n887_130_nodetypeSplitedNodeRoot.hpp"
#include "../../header/n887_nodeType/n887_500_nodetypePrograms.hpp"


using namespace std;


extern const InFrontMaskBb g_inFrontMaskBb;
extern NodetypeAbstract* g_NODETYPE_PROGRAMS[];
extern RepetitionTypeArray g_repetitionTypeArray;


NodetypeSplitedNodeRoot g_NODETYPE_SPLITEDNODE_ROOT;


//*
ScoreIndex NodetypeSplitedNodeRoot::GoToTheAdventure_new(
	Rucksack& rucksack,
	Position& pos,
	Flashlight* pFlashlight,//サーチスタック
	ScoreIndex alpha,
	ScoreIndex beta,
	const Depth depth,
	const bool cutNode
	) const {

	assert(-ScoreInfinite <= alpha && alpha < beta && beta <= ScoreInfinite);
	this->AssertBeforeStep1(
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
	this->DoStep1a(
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

	this->DoStep1b(
		bestScore,
		&pFlashlight,
		threatMove,
		bestMove
		);
	this->DoStep1c(
		&pThisThread,
		pFlashlight
		);

	bool isReturnWithScore = false;
	ScoreIndex returnScore = ScoreIndex::ScoreNone;

	// step2
	this->DoStep2(
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
	this->DoStep3(
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
	this->DoStep4(
		excludedMove,
		&pFlashlight,
		posKey,
		pos,
		&pTtEntry,//セットされる☆
		rucksack,
		ttScore
		);
	this->DoStep4x(
		ttMove,
		rucksack,
		pTtEntry,
		pos
		);
	this->DoStep4y(
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
	this->DoStep4z(
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
	this->DoStep5(
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
	this->DoStep6_NonPV(
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
	this->DoStep7(
		isReturnWithScore,
		returnScore,
		&pFlashlight,
		depth,
		beta,
		eval
		);

	// step8
	this->DoStep8_NonPV(
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
	this->DoStep9(
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
	this->DoStep10(
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
		this->GetBetaAtStep11(beta)//PVノードか、そうでないかで初期値を変えるぜ☆（＾ｑ＾）
		);
	const CheckInfo ci(pos);

	this->DoStep11a_BeforeLoop_SplitPointStart(
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
			move = this->GetMoveAtStep11(mp)
			).IsNone()
		) {

		bool isContinue = false;
		this->DoStep11b_LoopHeader(
			isContinue,
			move,
			excludedMove
			);
		if (isContinue)
		{
			continue;
		}

		this->DoStep11c_LoopHeader(
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

		this->DoStep11d_LoopHeader(
			isContinue,
			rucksack,
			move
			);
		if (isContinue)
		{
			continue;
		}

		this->DoStep11e_LoopHeader(
			rucksack,
			moveCount
			);

		this->DoStep11f_LoopHeader(
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
		this->DoStep12(
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
		this->DoStep13a(
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
		this->DoStep13b(
			pos,
			move,
			ci,
			moveCount,
			isContinue
			);
		this->DoStep13c(
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
		this->DoStep13d(
			captureOrPawnPromotion,
			playedMoveCount,
			movesSearched,
			move
			);

		// step14
		this->DoStep14(
			pos,
			move,
			st,
			ci,
			givesCheck,
			&pFlashlight
			);

		// step15
		this->DoStep15(
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
		this->DoStep16a(
			doFullDepthSearch,
			alpha,
			&pSplitedNode
			);
		this->DoStep16b_NonPVAtukai(
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
		this->DoStep16c(
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
		this->DoStep17(
			pos,
			move
			);

		assert(-ScoreInfinite < score && score < ScoreInfinite);

		// step18
		this->DoStep18a(
			&pSplitedNode,
			bestScore,
			alpha
			);

		if (rucksack.m_signals.m_stop || pThisThread->CutoffOccurred()) {
			return score;
		}

		this->DoStep18b(
			rucksack,
			move,
			isPVMove,
			alpha,
			score,
			pos
			);
		bool isBreak = false;
		this->DoStep18c(
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
		this->DoStep19(
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
			cutNode
			);
		if (isBreak) {
			break;
		}
	}

	if (this->GetReturnBeforeStep20()) {
		return bestScore;
	}

	// step20
	this->DoStep20(
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
//*/