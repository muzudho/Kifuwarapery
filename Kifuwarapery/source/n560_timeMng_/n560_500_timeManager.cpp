#include <algorithm> // std::min
#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n560_timeMng_/n560_100_limitsOfThinking.hpp"
#include "../../header/n560_timeMng_/n560_150_TimeType.hpp"
#include "../../header/n560_timeMng_/n560_500_timeManager.hpp"
#include "../../header/n885_searcher/n885_040_rucksack.hpp"


namespace {
#if 1
	//────────────────────────────────────────────────────────────────────────────────
	// MaxRatio …… 1.0 より大きくする感じ☆？ 残り時間に何か影響する☆？
	//
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 800.0f; // 試しに１６０倍にするとどうなるんだぜ☆？（＾ｑ＾）？
								   // 1手に 17秒ぐらいかけてくれて、序盤に長考してくれていい感じ☆（＾ｑ＾）
								   //────────────────────────────────────────────────────────────────────────────────
#elif 9
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 8000.0f; // 試しに１６００倍にするとどうなるんだぜ☆？（＾ｑ＾）？
									// 40手目まで自分の時間をあんまり使わなくなってしまった印象だぜ☆（＾ｑ＾） 800.f の方がマシかも☆
									//────────────────────────────────────────────────────────────────────────────────
#elif 8
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 80.0f; // 試しに１６倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 1手に 13秒ぐらいかけて考えてくれるようになったぜ☆（＾ｑ＾）
	// いい感じ☆（＾＿＾）
#elif 7
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 40.0f; // 試しに８倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 10秒も考えずに　7秒ぐらいで　ぽんぽん指している感じだったぜ☆
#elif 6
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 20.0f; // 試しに４倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 通常で１分読む感じ☆　最後に３分使い切らずに余してしまう。
#elif 5
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 10.0f; // 試しに２倍にするとどうなるんだぜ☆？（＾ｑ＾）？
	// 普段 20秒ぐらいで指す感じ☆　長考で 1分、相手の思考時間も合わせての大長考で 4分ぐらい☆
	// 投了時に 3分ぐらい残ってしまう感じ☆
#elif 4
	//────────────────────────────────────────────────────────────────────────────────
	// フィッシャールール 10分 加算10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 5.0f; // 秒は使っていいかもだぜ☆（＾ｑ＾）
#elif 3
	//────────────────────────────────────────────────────────────────────────────────
	// 10分 秒読み10秒用にはちょうど良かったぜ☆（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 2.5; // 2.0 は減らしすぎたかだぜ☆（＾ｑ＾）
#elif 2
	//────────────────────────────────────────────────────────────────────────────────
	// 10分 秒読み10秒用にするには☆？（＾ｑ＾）
	const int g_MOVE_HORIZON = 47;
	const float g_MAX_RATIO = 2.0; // 適当に減らすぜ☆（＾ｑ＾）→30秒ぐらいかけて 36手まで読むようになったぜ☆
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
	const int g_MOVE_HORIZON = 35; // 2時間切れ負け用。(todo: もう少し時間使っても良いかも知れない。)
	const float g_MAX_RATIO = 5.0; // 2時間切れ負け用。
#endif
	// スティール率とは何なのか☆？（＾ｑ＾）？
	const float g_STEAL_RATIO = 0.33;

	// 手目毎の思考時間の重み☆　もともと 1手目～512手目☆（＾ｑ＾）半分に減らそうぜ☆
	// Stockfish とは異なる。
	const int g_TEME_OMOMI_NUM = 512;
	// 旧名：ＭｏｖｅＩｍｐｏｒｔａｎｃｅ
	const int g_TEME_OMOMI[g_TEME_OMOMI_NUM] = {
		/*
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
		*/
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
		293 , 287 , 284 , 281 , 277 , 273 , 265 , 261 , 255 , 251 , 247 , 241 , 240 , 235 , 229 , 218 ,	// ここで256手目☆（＾ｑ＾）ここまでを使う☆
		// この先は 現在＋256手先まで読む設定に対応するための、累計に影響を与えないゼロ値☆
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
	};

	// 旧名：ｇｅｔＭｏｖｅＩｍｐｏｒｔａｎｃｅ
	int G_GetTemeOmomi(const Ply ply) {
		// ply は多くても 511 まで☆（配列のリミットチェック）（＾ｑ＾）
		return g_TEME_OMOMI[std::min(ply, g_TEME_OMOMI_NUM-1)];//511
	}

	// 次回の残り時間☆
	template <TimeType T> int G_Remaining(
		const int myTime,		// これが叩き台となる時間だぜ☆（＾ｑ＾）
		const int movesToGo,	//ムーブ・ホライズン以下の値☆（＾ｑ＾）
		const Ply currentPly,
		const int slowMover
	) {
		const float tMaxRatio   = (T == OptimumTime ? 1 : g_MAX_RATIO); // 予定思考時間なら 1、最大延長時間なら 定数指定☆
		const float tStealRatio = (T == OptimumTime ? 0 : g_STEAL_RATIO); // 予定思考時間なら 0、最大延長時間なら 定数指定☆

		// 旧名：ｔｈｉｓＭｏｖｅＩｍｐｏｒｔａｎｃｅ
		// 現在の手の、ムーブ・インポータンス☆（＾ｑ＾）
		const float currentOmomi = G_GetTemeOmomi(currentPly) * slowMover / 100;
		// スロー・ムーバーの設定が 100 のときは、moveImportance そのまんまの値になるぜ☆（＾ｑ＾）

		// 旧名：ｏｔｈｅｒＭｏｖｅＩｍｐｏｒｔａｎｃｅ
		// 残りの手の、ムーブ・インポータンスの合計☆（＾ｑ＾）
		float nokoriOmomiRuikei = 0;

		for (int i = 1; i < movesToGo; ++i) {
			// 現在の手数から、白黒手番の数×ループ回数分の moveImportance を足しこみだぜ☆（＾ｑ＾）
			// 自分の手番だけを足したいので、1つ飛ばしだぜ☆（＾ｑ＾）
			nokoriOmomiRuikei += G_GetTemeOmomi(currentPly + 2 * i);
		}

		// ２種類の比率を調べて、小さい方を取るぜ☆（＾ｑ＾）
		const float ratio1 =
			(tMaxRatio * currentOmomi)
			/
			// １／１の分母に　残り重み累計　を加算した形☆
			static_cast<float>(tMaxRatio * currentOmomi + nokoriOmomiRuikei);

		const float ratio2 =
			// 分子の重みに スティール比率 を掛けた形☆
			(tStealRatio * nokoriOmomiRuikei + currentOmomi)
			/
			static_cast<float>(nokoriOmomiRuikei + currentOmomi);

		return static_cast<int>(myTime * std::min(ratio1, ratio2));
	}
}



// 旧名：ＳｅｔＰｖＩｎｓｔａｂｉｌｉｔｙ
void TimeManager::SetPvInstability(
	const int currChanges,
	const int prevChanges
) {
	this->SetSikoAsobiTime(
		currChanges * (this->GetYoteiSikoTime() / 2)
		+
		prevChanges * (this->GetYoteiSikoTime() / 3)
		);
}




void TimeManager::InitializeTimeManager_OnHitchhikerThinkStarted(
	bool& isMoveTime0Clear,// false を入れてくれ☆（＾ｑ＾）
	const LimitsOfThinking& limits, // m_moveTimeを 0にする場合があるぜ☆（＾ｑ＾）
	const Ply currentPly,
	const Color us,
	Rucksack* pRucksack
	) {

	const int emergencyMoveHorizon = pRucksack->m_engineOptions["Emergency_Move_Horizon"];
	//const int emergencyBaseTime    = pRucksack->m_engineOptions["Emergency_Base_Time"];	// 緊急時用に残しておこうというタイム（ミリ秒）か☆？
	//const int emergencyMoveTime    = pRucksack->m_engineOptions["Emergency_Move_Time"];	// 緊急時用に残しておこうというタイム（ミリ秒）か☆？
	const int minThinkingTime      = pRucksack->m_engineOptions["Minimum_Thinking_Time"];	// １手につき、最低限の思考時間☆
    const int slowMover            = pRucksack->m_engineOptions["Slow_Mover"];

	this->ZeroClearSikoAsobiTime();
	this->SetYoteiSikoTime( limits.GetNokoriTime(us));// 予定思考時間は、残り時間をそのまんま入れて初期化☆？（＾ｑ＾）？
	this->SetSaidaiEnchoTime( limits.GetNokoriTime(us));// 最大延長時間も☆？（＾ｑ＾）？

	for (
		// ムーブス・ツー・ゴー
		int iHypMtg = 1; // ループ・カウンター☆
		iHypMtg <= (
			limits.GetMovesToGo() ?
			// 0以外なら、最大でもムーブ・ホライゾンまで☆
			std::min(limits.GetMovesToGo(), g_MOVE_HORIZON)
			:
			// 0なら、ムーブ・ホライゾンまで☆
			g_MOVE_HORIZON
		);
		++iHypMtg
	) {
		// 元の名前：ｈｙｐＭｙＴｉｍｅ
		// マイ・タイム☆　叩き台となる時間だぜ☆（＾ｑ＾）
		int tatakidaiTime =
			limits.GetNokoriTime(us)
			+ limits.GetIncrement(us) * (iHypMtg - 1)	// 今後追加されるインクリメントの累計☆
			//- emergencyBaseTime	// 緊急時用に残しておこうというタイム（ミリ秒）か☆？
			//- emergencyMoveTime	// 緊急時用に残しておこうというタイム（ミリ秒）か☆？
			//+ std::min(iHypMtg, emergencyMoveHorizon) // 1～255 ミリ秒を加算してどうするのか☆？（＾ｑ＾）
			;

		tatakidaiTime = std::max(tatakidaiTime, 0); // 0以上を確保するぜ☆（＾ｑ＾）

		// 思考予定タイムが、少なくなっていれば更新します。
		this->SmallUpdate_YoteiSikoTime(
			minThinkingTime + G_Remaining<OptimumTime>(tatakidaiTime, iHypMtg, currentPly, slowMover)// 思考時間＋残り時間
			); 

		// 最大延長タイムが、少なくなっていれば更新します。
		this->SmallUpdate_SaidaiEnchoTime(
			minThinkingTime + G_Remaining<MaxTime>(tatakidaiTime, iHypMtg, currentPly, slowMover) // 思考時間＋残り時間
		);
	}

	if (pRucksack->m_engineOptions["USI_Ponder"]) {
		// ポンダーを使う設定☆？（＾ｑ＾）？
		// 思考予定タイムには、 4分の1　のボーナスを追加します。
		this->IncreaseYoteiSikoTime( this->GetYoteiSikoTime() / 4);
	}

	// 大きくなっていれば更新します。
	this->LargeUpdate_YoteiSikoTime( minThinkingTime);
	// 小さくなっていれば更新します。
	this->SmallUpdate_YoteiSikoTime( this->GetSaidaiEnchoTime() );

	if (limits.GetMoveTime() != 0) {//（＾ｑ＾）いつも　０　な気がするぜ☆
		// こんなとこ、実行されないんじゃないかだぜ☆？（＾ｑ＾）？
		if (this->GetYoteiSikoTime() < limits.GetMoveTime()) {
			this->SetYoteiSikoTime( std::min(limits.GetNokoriTime(us), limits.GetMoveTime()));
		}
		if (this->GetSaidaiEnchoTime() < limits.GetMoveTime()) {
			this->SetSaidaiEnchoTime( std::min(limits.GetNokoriTime(us), limits.GetMoveTime()) );
		}
		this->IncreaseYoteiSikoTime( limits.GetMoveTime());
		this->IncreaseSaidaiEnchoTime( limits.GetMoveTime());
		if (limits.GetNokoriTime(us) != 0) {
			isMoveTime0Clear = true;
		}
	}
	//旧表示：optimum_search_time
	//旧表示：maximum_search_time	
	SYNCCOUT << "info string limits movesToGo " << limits.GetMovesToGo() << SYNCENDL;
	SYNCCOUT << "info string tukatteii time " << this->GetTukatteiiTime() << " ( yotei " << this->GetYoteiSikoTime() << " + asobi " << this->GetSikoAsobiTime() << ") / saidai encho " << this->GetSaidaiEnchoTime() << SYNCENDL;
}
