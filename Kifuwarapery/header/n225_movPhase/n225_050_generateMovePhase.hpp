#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"


// 指し手生成フェーズ☆？（＾ｑ＾）320_540_ＭｏｖｅＰｉｃｋｅｒとかで使う☆
enum GenerateMovePhase {
	N00_MainSearch,
	N01_PH_TacticalMoves0,
	N02_PH_Killers,
	N03_PH_NonTacticalMoves0,
	N04_PH_NonTacticalMoves1,
	N05_PH_BadCaptures,
	N06_EvasionSearch,
	N07_PH_Evasions,
	N08_QSearch,
	N09_PH_QCaptures0,
	N10_QEvasionSearch,
	N11_PH_QEvasions,
	N12_ProbCut,
	N13_PH_TacticalMoves1,
	N14_QRecapture,
	N15_PH_QCaptures1,
	N16_PH_Stop,
	GenerateMovePhaseNum	//(^q^)追加☆
};
OverloadEnumOperators(GenerateMovePhase); // ++phase_ の為。
