﻿#include <algorithm> // std::min
#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n560_timeMng_/n560_100_limitsOfThinking.hpp"
#include "../../header/n560_timeMng_/n560_500_timeManager.hpp"
#include "../../header/n885_searcher/n885_500_rucksack.hpp"


namespace {
#if 1
	//────────────────────────────────────────────────────────────────────────────────
	// MaxRatio …… 1.0 より大きくする感じ☆？ 残り時間に何か影響する☆？
	//
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int MoveHorizon = 47;
	const float MaxRatio = 80.0f; // 試しに１６倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 1手に 13秒ぐらいかけて考えてくれるようになったぜ☆（＾ｑ＾）
	// いい感じ☆（＾＿＾）
#elif 7
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int MoveHorizon = 47;
	const float MaxRatio = 40.0f; // 試しに８倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 10秒も考えずに　7秒ぐらいで　ぽんぽん指している感じだったぜ☆
#elif 6
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int MoveHorizon = 47;
	const float MaxRatio = 20.0f; // 試しに４倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 通常で１分読む感じ☆　最後に３分使い切らずに余してしまう。
#elif 5
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int MoveHorizon = 47;
	const float MaxRatio = 10.0f; // 試しに２倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 普段 20秒ぐらいで指す感じ☆　長考で 1分、相手の思考時間も合わせての大長考で 4分ぐらい☆
	// 投了時に 3分ぐらい残ってしまう感じ☆
#elif 4
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int MoveHorizon = 47;
	const float MaxRatio = 5.0f; // 秒は使っていいかもだぜ☆（＾ｑ＾）
#elif 3
	//────────────────────────────────────────────────────────────────────────────────
	// 10分 秒読み10秒用にはちょうど良かったぜ☆（＾ｑ＾）
	const int MoveHorizon = 47;
	const float MaxRatio = 2.5; // 2.0 は減らしすぎたかだぜ☆（＾ｑ＾）
#elif 2
	//────────────────────────────────────────────────────────────────────────────────
	// 10分 秒読み10秒用にするには☆？（＾ｑ＾）
	const int MoveHorizon = 47;
	const float MaxRatio = 2.0; // 適当に減らすぜ☆（＾ｑ＾）→30秒ぐらいかけて 36手まで読むようになったぜ☆
								// 後半まで時間を残すのもいい感じだぜ☆（＾ｑ＾）
								// 序盤、16秒ぐらいしか考えなくなったので序盤に悪くしてしまうぜ☆（＾ｑ＾）
	//────────────────────────────────────────────────────────────────────────────────
	// 15分切れ負け用。
	// (^q^)10分だと22秒ぐらいかけて 28～31手先まで読んでる感じ☆？
	//const int MoveHorizon = 47;
	//const float MaxRatio = 3.0;
	//────────────────────────────────────────────────────────────────────────────────
	//const float MaxRatio = 5.0; // 15分 秒読み10秒用。
	//────────────────────────────────────────────────────────────────────────────────
#else
	const int MoveHorizon = 35; // 2時間切れ負け用。(todo: もう少し時間使っても良いかも知れない。)
	const float MaxRatio = 5.0; // 2時間切れ負け用。
#endif
	const float StealRatio = 0.33;

	// Stockfish とは異なる。
	const int MoveImportance[512] = {
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780, 7780,
		7780, 7780, 7780, 7780, 7778, 7778, 7776, 7776, 7776, 7773, 7770, 7768, 7766, 7763, 7757, 7751,
		7743, 7735, 7724, 7713, 7696, 7689, 7670, 7656, 7627, 7605, 7571, 7549, 7522, 7493, 7462, 7425,
		7385, 7350, 7308, 7272, 7230, 7180, 7139, 7094, 7055, 7010, 6959, 6902, 6841, 6778, 6705, 6651,
		6569, 6508, 6435, 6378, 6323, 6253, 6152, 6085, 5995, 5931, 5859, 5794, 5717, 5646, 5544, 5462,
		5364, 5282, 5172, 5078, 4988, 4901, 4831, 4764, 4688, 4609, 4536, 4443, 4365, 4293, 4225, 4155,
		4085, 4005, 3927, 3844, 3765, 3693, 3634, 3560, 3479, 3404, 3331, 3268, 3207, 3146, 3077, 3011,
		2947, 2894, 2828, 2776, 2727, 2676, 2626, 2589, 2538, 2490, 2442, 2394, 2345, 2302, 2243, 2192,
		2156, 2115, 2078, 2043, 2004, 1967, 1922, 1893, 1845, 1809, 1772, 1736, 1702, 1674, 1640, 1605,
		1566, 1536, 1509, 1479, 1452, 1423, 1388, 1362, 1332, 1304, 1289, 1266, 1250, 1228, 1206, 1180,
		1160, 1134, 1118, 1100, 1080, 1068, 1051, 1034, 1012, 1001, 980 , 960 , 945 , 934 , 916 , 900 ,
		888 , 878 , 865 , 852 , 828 , 807 , 787 , 770 , 753 , 744 , 731 , 722 , 706 , 700 , 683 , 676 ,
		671 , 664 , 652 , 641 , 634 , 627 , 613 , 604 , 591 , 582 , 568 , 560 , 552 , 540 , 534 , 529 ,
		519 , 509 , 495 , 484 , 474 , 467 , 460 , 450 , 438 , 427 , 419 , 410 , 406 , 399 , 394 , 387 ,
		382 , 377 , 372 , 366 , 359 , 353 , 348 , 343 , 337 , 333 , 328 , 321 , 315 , 309 , 303 , 298 ,
		293 , 287 , 284 , 281 , 277 , 273 , 265 , 261 , 255 , 251 , 247 , 241 , 240 , 235 , 229 , 218
	};

	int moveImportance(const Ply ply) {
		return MoveImportance[std::min(ply, 511)];
	}

	enum TimeType {
		OptimumTime,
		MaxTime
	};

	// 残り時間☆
	template <TimeType T> int remaining(
		const int myTime, const int movesToGo, const Ply currentPly, const int slowMover
	) {
		const float TMaxRatio   = (T == OptimumTime ? 1 : MaxRatio);
		const float TStealRatio = (T == OptimumTime ? 0 : StealRatio);

		const float thisMoveImportance = moveImportance(currentPly) * slowMover / 100;
		float otherMoveImportance = 0;

		for (int i = 1; i < movesToGo; ++i) {
			otherMoveImportance += moveImportance(currentPly + 2 * i);
		}

		const float ratio1 =
			(TMaxRatio * thisMoveImportance) / static_cast<float>(TMaxRatio * thisMoveImportance + otherMoveImportance);
		const float ratio2 =
			(thisMoveImportance + TStealRatio * otherMoveImportance) / static_cast<float>(thisMoveImportance + otherMoveImportance);

		return static_cast<int>(myTime * std::min(ratio1, ratio2));
	}
}

void TimeManager::SetPvInstability(const int currChanges, const int prevChanges) {
	m_unstablePVExtraTime_ =
		currChanges * (m_optimumSearchTime_ / 2)
		+
		prevChanges * (m_optimumSearchTime_ / 3);
}

void TimeManager::Init(LimitsOfThinking& limits, const Ply currentPly, const Color us, Rucksack* searcher) {
	const int emergencyMoveHorizon = searcher->m_engineOptions["Emergency_Move_Horizon"];
	const int emergencyBaseTime    = searcher->m_engineOptions["Emergency_Base_Time"];
	const int emergencyMoveTime    = searcher->m_engineOptions["Emergency_Move_Time"];
	const int minThinkingTime      = searcher->m_engineOptions["Minimum_Thinking_Time"];
    const int slowMover            = searcher->m_engineOptions["Slow_Mover"];

	m_unstablePVExtraTime_ = 0;
	m_optimumSearchTime_ = m_maximumSearchTime_ = limits.m_time[us];

	for (
		// ムーブス・ツー・ゴー
		int hypMTG = 1;
		hypMTG <= (limits.m_movesToGo ? std::min(limits.m_movesToGo, MoveHorizon) : MoveHorizon);
		++hypMTG
	) {
		// マイ・タイム
		int hypMyTime =
			limits.m_time[us]
			+ limits.m_increment[us] * (hypMTG - 1)
			- emergencyBaseTime
			- emergencyMoveTime + std::min(hypMTG, emergencyMoveHorizon);

		hypMyTime = std::max(hypMyTime, 0);

		// 少ない方に更新します。
		this->m_optimumSearchTime_ = std::min(
			this->m_optimumSearchTime_,
			minThinkingTime + remaining<OptimumTime>(hypMyTime, hypMTG, currentPly, slowMover)
		);

		// 少ない方に更新します。
		this->m_maximumSearchTime_ = std::min(
			this->m_maximumSearchTime_,
			minThinkingTime + remaining<MaxTime>(hypMyTime, hypMTG, currentPly, slowMover)
		);
	}

	if (searcher->m_engineOptions["USI_Ponder"]) {
		// 4分の1を追加します。
		this->m_optimumSearchTime_ += this->m_optimumSearchTime_ / 4;
	}

	// こちらも min 以上 max 以下☆にする。
	this->m_optimumSearchTime_ = std::max(this->m_optimumSearchTime_, minThinkingTime);
	this->m_optimumSearchTime_ = std::min(this->m_optimumSearchTime_, m_maximumSearchTime_);

	if (limits.m_moveTime != 0) {
		if (this->m_optimumSearchTime_ < limits.m_moveTime) {
			this->m_optimumSearchTime_ = std::min(limits.m_time[us], limits.m_moveTime);
		}
		if (m_maximumSearchTime_ < limits.m_moveTime) {
			m_maximumSearchTime_ = std::min(limits.m_time[us], limits.m_moveTime);
		}
		this->m_optimumSearchTime_ += limits.m_moveTime;
		this->m_maximumSearchTime_ += limits.m_moveTime;
		if (limits.m_time[us] != 0) {
			limits.m_moveTime = 0;
		}
	}
	SYNCCOUT << "info string optimum_search_time = " << this->m_optimumSearchTime_ << SYNCENDL;
	SYNCCOUT << "info string maximum_search_time = " << this->m_maximumSearchTime_ << SYNCENDL;
}