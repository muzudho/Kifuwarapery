#include <algorithm> // std::min
#include "../../header/n119_score___/n119_090_scoreIndex.hpp"
#include "../../header/n560_timeMng_/n560_100_limitsOfThinking.hpp"
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

	// 旧名：ｒｅｍａｉｎｉｎｇ
	// 次回の残り時間☆
	inline float G_TimeBairitu_YoteiSiko(
		const int movesToGo	//ムーブ・ホライズン以下の値☆（＾ｑ＾）
	) {
		// 旧名：ｏｔｈｅｒＭｏｖｅＩｍｐｏｒｔａｎｃｅ
		// 残りの手の、ムーブ・インポータンスの合計☆（＾ｑ＾）
		float nokoriOmomiRuikei = movesToGo - 1;
		if (nokoriOmomiRuikei < 0) {
			nokoriOmomiRuikei = 0;
		}

		//   １
		// ───────
		// 1 + 累計
		return 
			1
			/
			static_cast<float>(1 + nokoriOmomiRuikei);
	}


	// スティール率とは何なのか☆？（＾ｑ＾）？
	const float g_STEAL_RATIO = 0.33;

	inline float G_TimeBairitu_SaidaiEncho(
		const int movesToGo	//ムーブ・ホライズン以下の値☆（＾ｑ＾）
		) {

		// 旧名：ｏｔｈｅｒＭｏｖｅＩｍｐｏｒｔａｎｃｅ
		// 残りの手の、ムーブ・インポータンスの合計☆（＾ｑ＾）
		float nokoriOmomiRuikei = movesToGo - 1;
		if (nokoriOmomiRuikei < 0) {
			nokoriOmomiRuikei = 0;
		}

		// ２種類の比率を調べて、小さい方を取るぜ☆（＾ｑ＾）

		//   比率
		// ───────
		// 比率 + 累計
		const float ratio1 =
			g_MAX_RATIO
			/
			static_cast<float>(g_MAX_RATIO + nokoriOmomiRuikei);

		//   スティール率 * 累計 + 1
		// ───────────────────────────
		//           累計 + 1
		const float ratio2 =
			(g_STEAL_RATIO * nokoriOmomiRuikei + 1)
			/
			static_cast<float>(nokoriOmomiRuikei + 1);

		return std::min(ratio1, ratio2);
	}
}



// 旧名：ＳｅｔＰｖＩｎｓｔａｂｉｌｉｔｙ
void TimeManager::SetPvInstability_AtIterativeDeepeningStarted(
	const Ply currBestMovePlyChanges,
	const Ply prevBestMovePlyChanges
) {
	this->SetSikoAsobiTime(
		currBestMovePlyChanges * (this->GetYoteiSikoTime() / 2)
		+
		prevBestMovePlyChanges * (this->GetYoteiSikoTime() / 3)
		);
}




void TimeManager::InitializeTimeManager_OnHitchhikerThinkStarted(
	bool& isMoveTime0Clear,// false を入れてくれ☆（＾ｑ＾）
	const LimitsOfThinking& limits, // m_moveTimeを 0にする場合があるぜ☆（＾ｑ＾）
	//const Ply currentPly,
	const Color us,
	Rucksack* pRucksack
	) {

	const int emergencyMoveHorizon = pRucksack->m_engineOptions["Emergency_Move_Horizon"];
	const int minThinkingTime      = pRucksack->m_engineOptions["Minimum_Thinking_Time"];	// １手につき、最低限の思考時間☆

	this->ZeroClearSikoAsobiTime();
	this->SetYoteiSikoTime( limits.GetNokoriTime(us));// 予定思考時間は、残り時間をそのまんま入れて初期化☆？（＾ｑ＾）？
	this->SetSaidaiEnchoTime( limits.GetNokoriTime(us));// 最大延長時間も☆？（＾ｑ＾）？

	for (
		// ムーブス・ツー・ゴー
		int iHypMtg = 1; // ループ・カウンター☆
		iHypMtg <= g_MOVE_HORIZON;
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
			static_cast<int>(tatakidaiTime * G_TimeBairitu_YoteiSiko(iHypMtg)) + minThinkingTime// 残り時間 × 倍率 + 最低思考時間
			); 

		// 最大延長タイムが、少なくなっていれば更新します。
		this->SmallUpdate_SaidaiEnchoTime(
			static_cast<int>(tatakidaiTime * G_TimeBairitu_SaidaiEncho(iHypMtg)) + minThinkingTime// 残り時間 × 倍率 + 最低思考時間
		);
	}

	if (pRucksack->m_engineOptions["USI_Ponder"]) {
		// 相手の思考中にも読み続ける設定の場合☆（＾ｑ＾）

		// 相手が何秒考えるかなんて分からないので☆（＾～＾）
		// 思考予定タイムには、 4分の1　のボーナスを追加しておくことにするぜ☆（＾▽＾）
		this->IncreaseYoteiSikoTime( this->GetYoteiSikoTime() / 4);
	}

	// 予定思考時間よりも最低思考時間の方が大きいようなら、予定思考時間は最低思考時間と等しくします。
	this->LargeUpdate_YoteiSikoTime( minThinkingTime);
	// 予定思考時間よりも最大延長時間の方が小さいようなら、予定思考時間は最大延長時間と等しくします。
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
	SYNCCOUT << "info string old limits move time " << limits.GetMoveTime() << SYNCENDL;
	// SYNCCOUT << "info string limits inc time " << limits.GetIncrement(us) << SYNCENDL; // 加算時間はちゃんと取得できていたぜ☆
	SYNCCOUT << "info string tukatteii time " << this->GetTukatteiiTime() << " ( yotei " << this->GetYoteiSikoTime() << " + asobi " << this->GetSikoAsobiTime() << ") / saidai encho " << this->GetSaidaiEnchoTime() << SYNCENDL;
}
