#include "../../header/n450_movPhase/n450_100_N00_mainSearch.hpp"
#include "../../header/n450_movPhase/n450_110_N01___TacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_120_N02___Killers.hpp"
#include "../../header/n450_movPhase/n450_130_N03___NonTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_140_N04___NonTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_150_N05___BadCaptures.hpp"
#include "../../header/n450_movPhase/n450_160_N06_evasionSearch.hpp"
#include "../../header/n450_movPhase/n450_170_N07___Evasions.hpp"
#include "../../header/n450_movPhase/n450_180_N08_qSearch.hpp"
#include "../../header/n450_movPhase/n450_190_N09___QCaptures0.hpp"
#include "../../header/n450_movPhase/n450_200_N10_qEvasionSearch.hpp"
#include "../../header/n450_movPhase/n450_210_N11___QEvasions.hpp"
#include "../../header/n450_movPhase/n450_220_N12_probCut.hpp"
#include "../../header/n450_movPhase/n450_230_N13___TacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_240_N14_qRecapture.hpp"
#include "../../header/n450_movPhase/n450_250_N15___QCaptures1.hpp"
#include "../../header/n450_movPhase/n450_260_N16___Stop.hpp"


MovePhaseAbstract* g_movePhaseArray[g_MOVEGENERATOR_PHASE_NUM] = {
	&g_mainSearch,
	&g_phTacticalMoves0,
	&g_phKillers,
	&g_phNonTacticalMoves0,
	&g_phNonTacticalMoves1,
	&g_phBadCaptures,
	&g_evasionSearch,
	&g_phEvasions,
	&g_qSearch,
	&g_phQCaptures0,
	&g_qEvasionSearch,
	&g_phQEvasions,
	&g_probCut,
	&g_phTacticalMoves1,
	&g_qRecapture,
	&g_phQCaptures1,
	&g_phStop
	//&g_movePhaseNum
};
