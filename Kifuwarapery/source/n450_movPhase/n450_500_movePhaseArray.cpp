#include "../../header/n450_movPhase/n450_100_mainSearch.hpp"
#include "../../header/n450_movPhase/n450_110_phTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_120_phKillers.hpp"
#include "../../header/n450_movPhase/n450_130_phNonTacticalMoves0.hpp"
#include "../../header/n450_movPhase/n450_140_phNonTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_150_phBadCaptures.hpp"
#include "../../header/n450_movPhase/n450_160_evasionSearch.hpp"
#include "../../header/n450_movPhase/n450_170_phEvasions.hpp"
#include "../../header/n450_movPhase/n450_180_qSearch.hpp"
#include "../../header/n450_movPhase/n450_190_phQCaptures0.hpp"
#include "../../header/n450_movPhase/n450_200_qEvasionSearch.hpp"
#include "../../header/n450_movPhase/n450_210_phQEvasions.hpp"
#include "../../header/n450_movPhase/n450_220_probCut.hpp"
#include "../../header/n450_movPhase/n450_230_phTacticalMoves1.hpp"
#include "../../header/n450_movPhase/n450_240_qRecapture.hpp"
#include "../../header/n450_movPhase/n450_250_phQCaptures1.hpp"
#include "../../header/n450_movPhase/n450_260_phStop.hpp"
#include "../../header/n450_movPhase/n450_260_phStop.hpp"


MovePhaseAbstract* g_movePhaseArray[GenerateMovePhase::GenerateMovePhaseNum] = {
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
