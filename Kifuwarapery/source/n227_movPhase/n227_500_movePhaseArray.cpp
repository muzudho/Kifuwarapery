#include "../../header/n227_movPhase/n227_100_mainSearch.hpp"
#include "../../header/n227_movPhase/n227_110_phTacticalMoves0.hpp"
#include "../../header/n227_movPhase/n227_120_phKillers.hpp"
#include "../../header/n227_movPhase/n227_130_phNonTacticalMoves0.hpp"
#include "../../header/n227_movPhase/n227_140_phNonTacticalMoves1.hpp"
#include "../../header/n227_movPhase/n227_150_phBadCaptures.hpp"
#include "../../header/n227_movPhase/n227_160_evasionSearch.hpp"
#include "../../header/n227_movPhase/n227_170_phEvasions.hpp"
#include "../../header/n227_movPhase/n227_180_qSearch.hpp"
#include "../../header/n227_movPhase/n227_190_phQCaptures0.hpp"
#include "../../header/n227_movPhase/n227_200_qEvasionSearch.hpp"
#include "../../header/n227_movPhase/n227_210_phQEvasions.hpp"
#include "../../header/n227_movPhase/n227_220_probCut.hpp"
#include "../../header/n227_movPhase/n227_230_phTacticalMoves1.hpp"
#include "../../header/n227_movPhase/n227_240_qRecapture.hpp"
#include "../../header/n227_movPhase/n227_250_phQCaptures1.hpp"
#include "../../header/n227_movPhase/n227_260_phStop.hpp"
#include "../../header/n227_movPhase/n227_260_phStop.hpp"


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
