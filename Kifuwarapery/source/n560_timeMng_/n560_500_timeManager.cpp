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
	// スティール率とは何なのか☆？（＾ｑ＾）？
	const float g_STEAL_RATIO = 0.33;

	// 旧名：ＭｏｖｅＩｍｐｏｒｔａｎｃｅ
	// Stockfish とは異なる。
	// 手目毎の時間配分の重み☆ 予定思考時間、最大延長時間の両方で使う☆（＾ｑ＾）
	// 大会は256手目までだぜ☆（＾ｑ＾）読みの配列インデックス・オーバーも考慮して、256手目の先も２倍分用意しておくぜ☆（＾ｑ＾）
	// 全部 0 にすると、おそらく残り時間＝予定思考時間＝最大延長時間　になるぜ☆（＾ｑ＾）
	const int g_SYOHI_OMOMI_BY_TEME_NUM = 512;

	const int g_SYOHI_OMOMI_BY_TEME_PLY016 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY032 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY048 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY064 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY080 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY096 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY112 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY128 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY144 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY160 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY176 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY192 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY208 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY224 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY240 = 1000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY256 = 1000;

	/* 序盤で1分も使う息切れタイプ☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY016 = 881467929;
	const int g_SYOHI_OMOMI_BY_TEME_PLY032 =  55091745;//  1/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY048 =   6886468;//  2/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY064 =   1291212;//  3/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY080 =    322803;//  4/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY096 =    100876;//  5/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY112 =     37828;//  6/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY128 =     16550;//  7/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY144 =      8275;//  8/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY160 =      4654;//  9/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY176 =      2909;// 10/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY192 =      2000;// 11/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY208 =      1500;// 12/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY224 =      1218;// 13/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY240 =      1066;// 14/16 にした☆
	const int g_SYOHI_OMOMI_BY_TEME_PLY256 =      1000;// 15/16 にした☆
	*/

	/* 序盤で40秒も使う序盤型になってしまったぜ☆（＾ｑ＾）息切れタイプ☆（＾ｑ＾）
	const int g_SYOHI_OMOMI_BY_TEME_PLY016 = 8815626;
	const int g_SYOHI_OMOMI_BY_TEME_PLY032 = 8264650;// 15/16 にした☆(-550976)
	const int g_SYOHI_OMOMI_BY_TEME_PLY048 = 7231569;// 14/16 にした☆(-1033081)
	const int g_SYOHI_OMOMI_BY_TEME_PLY064 = 5875650;// 13/16 にした☆(-1355919)
	const int g_SYOHI_OMOMI_BY_TEME_PLY080 = 4406738;// 12/16 にした☆(-1468912)
	const int g_SYOHI_OMOMI_BY_TEME_PLY096 = 3029633;// 11/16 にした☆(-1377105)
	const int g_SYOHI_OMOMI_BY_TEME_PLY112 = 1893521;// 10/16 にした☆(-1136112)
	const int g_SYOHI_OMOMI_BY_TEME_PLY128 = 1065106;//  9/16 にした☆(-828415)
	const int g_SYOHI_OMOMI_BY_TEME_PLY144 =  532553;//  8/16 にした☆(-532553)
	const int g_SYOHI_OMOMI_BY_TEME_PLY160 =  232992;//  7/16 にした☆(-299561)
	const int g_SYOHI_OMOMI_BY_TEME_PLY176 =   87372;//  6/16 にした☆(-145620)
	const int g_SYOHI_OMOMI_BY_TEME_PLY192 =   27304;//  5/16 にした☆(-60068)
	const int g_SYOHI_OMOMI_BY_TEME_PLY208 =    6826;//  4/16 にした☆(-20478)
	const int g_SYOHI_OMOMI_BY_TEME_PLY224 =    1280;//  3/16 にした☆(-5546)
	const int g_SYOHI_OMOMI_BY_TEME_PLY240 =     160;//  2/16 にした☆(-1120)
	const int g_SYOHI_OMOMI_BY_TEME_PLY256 =      10;//  1/16 にした☆(-150)
	*/

	/* ↓30手目で持ち時間半分使い切って、総矢倉とか定跡形を組んで評価値 -127 とかいいことないぜ☆（＾ｑ＾）
	const int g_SYOHI_OMOMI_BY_TEME_PLY016 = 100000;
	const int g_SYOHI_OMOMI_BY_TEME_PLY032 = 93750;// 次は 15/16 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY048 = 87500;// 次は 14/15 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY064 = 81250;// 次は 13/14 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY080 = 75000;// 次は 12/13 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY096 = 68750;// 次は 11/12 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY112 = 62500;// 次は 10/11 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY128 = 56250;// 次は  9/10 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY144 = 50000;// 次は  8/ 9 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY160 = 43750;// 次は  7/ 8 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY176 = 37500;// 次は  6/ 7 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY192 = 31250;// 次は  5/ 6 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY208 = 25000;// 次は  4/ 5 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY224 = 18750;// 次は  3/ 4 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY240 = 12500;// 次は  2/ 3 に☆(-6250)
	const int g_SYOHI_OMOMI_BY_TEME_PLY256 =  6250;// 次は  1/ 2 に☆(-6250)
	//*/

	const int g_SYOHI_OMOMI_BY_TEME[g_SYOHI_OMOMI_BY_TEME_NUM] = {
		//
		// カーブにしないとな☆（＾ｑ＾）
		//
		// 1～16
		g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016, g_SYOHI_OMOMI_BY_TEME_PLY016,
		// 17～32
		g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032, g_SYOHI_OMOMI_BY_TEME_PLY032,
		// 33～48
		g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048, g_SYOHI_OMOMI_BY_TEME_PLY048,
		// 49～64
		g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064, g_SYOHI_OMOMI_BY_TEME_PLY064,
		// 65～80
		g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080, g_SYOHI_OMOMI_BY_TEME_PLY080,
		// 81～96
		g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096, g_SYOHI_OMOMI_BY_TEME_PLY096,
		// 97～112
		g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112, g_SYOHI_OMOMI_BY_TEME_PLY112,
		// 113～128
		g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128, g_SYOHI_OMOMI_BY_TEME_PLY128,
		// 129～144
		g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144, g_SYOHI_OMOMI_BY_TEME_PLY144,
		// 145～160
		g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160, g_SYOHI_OMOMI_BY_TEME_PLY160,
		// 161～176
		g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176, g_SYOHI_OMOMI_BY_TEME_PLY176,
		// 177～192
		g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192, g_SYOHI_OMOMI_BY_TEME_PLY192,
		// 193～208
		g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208, g_SYOHI_OMOMI_BY_TEME_PLY208,
		// 209～224
		g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224, g_SYOHI_OMOMI_BY_TEME_PLY224,
		// 225～240
		g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240, g_SYOHI_OMOMI_BY_TEME_PLY240,
		// 241～256
		g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256, g_SYOHI_OMOMI_BY_TEME_PLY256,
		//────────────────────────────────────────────────────────────────────────────────
		// 以降手抜き☆
		//────────────────────────────────────────────────────────────────────────────────
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 1);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 2);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 3);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 4);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 5);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 6);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 7);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 8);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 9);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (10);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (11);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (12);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (13);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (14);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (15);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 	// (16);
		/*
		//
		// 逆にしてみたぜ☆（＾ｑ＾）１手目付近が小さく、２５６手目付近が大きく☆
		// これで　１手目付近は　１７秒ぐらい使っているぜ☆　逆にすると１手目付近から３０秒ぐらい使ってしまってタイヘンだったぜ☆（＾ｑ＾）
		//
		// FIXME: 150手目あたりで ５分残して 14秒 ぐらいしか使わなくなるぜ☆（＾ｑ＾） 20秒ぐらい使ってほしいのに☆
		//
		//────────────────────────────────────────────────────────────────────────────────
		// 1～32手目の重み☆ 4/3 ぐらい上昇☆
		//────────────────────────────────────────────────────────────────────────────────
		800,  800,  800,  800,  800,  800,  800,  800,  800,  800,  800,  800,  800,  800,  800,  800, // 16:
		933,  933,  933,  933,  933,  933,  933,  933,  933,  933,  933,  933,  933,  933,  933,  933, // 32: 32 手目まで定跡で 思考 10数秒 ということはよくある☆
																									   //────────────────────────────────────────────────────────────────────────────────
																									   // 33～64手目の重み☆ 4/3 ぐらい上昇☆
																									   //────────────────────────────────────────────────────────────────────────────────
		1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, 1067, // 48:
		1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425, 1425,	// 64:
																										//────────────────────────────────────────────────────────────────────────────────
																										// 80～96手目の重み☆ 4/3 ぐらい上昇☆
																										//────────────────────────────────────────────────────────────────────────────────
		1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423, 1423,	// 80:
		1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660, 1660,	// 96:
																										//────────────────────────────────────────────────────────────────────────────────
																										// 97～128手目の重み☆ 4/3 ぐらい上昇☆
																										//────────────────────────────────────────────────────────────────────────────────
		1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898, 1898,	// 112:
		2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, 2214, // 128:
																										//────────────────────────────────────────────────────────────────────────────────
																										// 129～160手目の重み☆ 4/3 ぐらい上昇☆
																										//────────────────────────────────────────────────────────────────────────────────
		2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, 2531, // 144:
		2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953, 2953,	// 160:
																										//────────────────────────────────────────────────────────────────────────────────
																										// 161～192手目の重み☆ 4/3 ぐらい上昇☆
																										//────────────────────────────────────────────────────────────────────────────────
		3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375, 3375,	// 176:
		3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937, 3937,	// 192:
																										//────────────────────────────────────────────────────────────────────────────────
																										// 193～224手目の重み☆ 4/3 ぐらい上昇☆
																										//────────────────────────────────────────────────────────────────────────────────
		4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500, 4500,	// 208:
		5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250, 5250,	// 224:
																										//────────────────────────────────────────────────────────────────────────────────
																										// 193～224手目の重み☆ 4/3 ぐらい上昇☆
																										//────────────────────────────────────────────────────────────────────────────────
		6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000, 6000,	// 240:
		7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, 7000, // 256: ここで256手目☆（＾ｑ＾）ここまでを使う☆
																										//────────────────────────────────────────────────────────────────────────────────
																										// 256手目以降は、現在＋256手先まで読む設定に対応するための、累計に影響を与えないダミーの 0 値☆
																										//────────────────────────────────────────────────────────────────────────────────
																										//────────────────────────────────────────────────────────────────────────────────
																										// 256～288手目の重み☆ 4/3 ぐらい上昇☆
																										//────────────────────────────────────────────────────────────────────────────────
		8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, // 272:
		8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000, 8000,	// 288;

																										//────────────────────────────────────────────────────────────────────────────────
																										// 以降手抜き☆
																										//────────────────────────────────────────────────────────────────────────────────
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 3);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 4);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 5);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 6);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 7);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 8);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// ( 9);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (10);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (11);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (12);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (13);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (14);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,	// (15);
		0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0 	// (16);
		//*/
	};

	// 旧名：ｇｅｔＭｏｖｅＩｍｐｏｒｔａｎｃｅ
	int G_GetSyohiOmomiByTeme(const Ply ply) {
		// ply は多くても 511 まで☆（配列のリミットチェック）（＾ｑ＾）
		return g_SYOHI_OMOMI_BY_TEME[std::min(ply, g_SYOHI_OMOMI_BY_TEME_NUM-1)];//511
	}

	// 旧名：ｒｅｍａｉｎｉｎｇ
	// 次回の残り時間☆
	inline float G_TimeBairitu_YoteiSiko(
		const int movesToGo,	//ムーブ・ホライズン以下の値☆（＾ｑ＾）
		const Ply currentPly,
		const int slowMover
	) {
		const float tMaxRatio   = 1; // 予定思考時間なら 1、最大延長時間なら 定数指定☆
		const float tStealRatio = 0; // 予定思考時間なら 0、最大延長時間なら 定数指定☆

		// 旧名：ｔｈｉｓＭｏｖｅＩｍｐｏｒｔａｎｃｅ
		// 現在の手の、ムーブ・インポータンス☆（＾ｑ＾）
		const float currentOmomi = G_GetSyohiOmomiByTeme(currentPly) * slowMover / 100;
		// スロー・ムーバーの設定が 100 のときは、moveImportance そのまんまの値になるぜ☆（＾ｑ＾）

		// 旧名：ｏｔｈｅｒＭｏｖｅＩｍｐｏｒｔａｎｃｅ
		// 残りの手の、ムーブ・インポータンスの合計☆（＾ｑ＾）
		float nokoriOmomiRuikei = 0;

		for (int i = 1; i < movesToGo; ++i) {
			// 現在の手数から、白黒手番の数×ループ回数分の moveImportance を足しこみだぜ☆（＾ｑ＾）
			// 自分の手番だけを足したいので、1つ飛ばしだぜ☆（＾ｑ＾）
			nokoriOmomiRuikei += G_GetSyohiOmomiByTeme(currentPly + 2 * i);
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

		return std::min(ratio1, ratio2);
	}

	inline float G_TimeBairitu_SaidaiEncho(
		const int movesToGo,	//ムーブ・ホライズン以下の値☆（＾ｑ＾）
		const Ply currentPly,
		const int slowMover
		) {
		const float tMaxRatio = g_MAX_RATIO; // 予定思考時間なら 1、最大延長時間なら 定数指定☆
		const float tStealRatio = g_STEAL_RATIO; // 予定思考時間なら 0、最大延長時間なら 定数指定☆

																		  // 旧名：ｔｈｉｓＭｏｖｅＩｍｐｏｒｔａｎｃｅ
																		  // 現在の手の、ムーブ・インポータンス☆（＾ｑ＾）
		const float currentOmomi = G_GetSyohiOmomiByTeme(currentPly) * slowMover / 100;
		// スロー・ムーバーの設定が 100 のときは、moveImportance そのまんまの値になるぜ☆（＾ｑ＾）

		// 旧名：ｏｔｈｅｒＭｏｖｅＩｍｐｏｒｔａｎｃｅ
		// 残りの手の、ムーブ・インポータンスの合計☆（＾ｑ＾）
		float nokoriOmomiRuikei = 0;

		for (int i = 1; i < movesToGo; ++i) {
			// 現在の手数から、白黒手番の数×ループ回数分の moveImportance を足しこみだぜ☆（＾ｑ＾）
			// 自分の手番だけを足したいので、1つ飛ばしだぜ☆（＾ｑ＾）
			nokoriOmomiRuikei += G_GetSyohiOmomiByTeme(currentPly + 2 * i);
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
			static_cast<int>(tatakidaiTime * G_TimeBairitu_YoteiSiko(iHypMtg, currentPly, slowMover)) + minThinkingTime// 残り時間 × 倍率 + 最低思考時間
			); 

		// 最大延長タイムが、少なくなっていれば更新します。
		this->SmallUpdate_SaidaiEnchoTime(
			static_cast<int>(tatakidaiTime * G_TimeBairitu_SaidaiEncho(iHypMtg, currentPly, slowMover)) + minThinkingTime// 残り時間 × 倍率 + 最低思考時間
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
