#pragma once

#include "../n080_common__/n080_150_overloadEnumOperators.hpp"

enum GenerateMovePhase {
	MainSearch, PH_TacticalMoves0, PH_Killers, PH_NonTacticalMoves0, PH_NonTacticalMoves1, PH_BadCaptures,
	EvasionSearch, PH_Evasions,
	QSearch, PH_QCaptures0,
	QEvasionSearch, PH_QEvasions,
	ProbCut, PH_TacticalMoves1,
	QRecapture, PH_QCaptures1,
	PH_Stop
};
OverloadEnumOperators(GenerateMovePhase); // ++phase_ �ׁ̈B
