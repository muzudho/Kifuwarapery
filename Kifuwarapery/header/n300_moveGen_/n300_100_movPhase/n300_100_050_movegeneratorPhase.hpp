#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


// 指し手生成フェーズ☆？（＾ｑ＾）320_540_ＭｏｖｅＰｉｃｋｅｒとかで使う☆
// このフェーズは順番に実行されるぜ☆（＾ｑ＾）
enum GenerateMovePhase {
	N00_MainSearch,// 最初のフェーズ☆
	N01_PH_TacticalMoves0,
	N02_PH_Killers,
	N03_PH_NonTacticalMoves0,
	N04_PH_NonTacticalMoves1,
	N05_PH_BadCaptures,
	N06_EvasionSearch,// 初期化で使われることもあるぜ☆
	N07_PH_Evasions,
	N08_QSearch,// 初期化で使われることもあるぜ☆
	N09_PH_QCaptures0,
	N10_QEvasionSearch,// 初期化で使われることもあるぜ☆
	N11_PH_QEvasions,
	N12_ProbCut,// 初期化で使われることもあるぜ☆
	N13_PH_TacticalMoves1,
	N14_QRecapture,// 初期化で使われることもあるぜ☆
	N15_PH_QCaptures1,
	N16_PH_Stop// 番兵☆（＾ｑ＾）処理を止める☆
	//GenerateMovePhaseNum	//(^q^)追加☆
};
OverloadEnumOperators(GenerateMovePhase); // ++phase_ の為。
static const int g_MOVEGENERATOR_PHASE_NUM = 17;