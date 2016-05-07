#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n300_moveGen_/n300_100_movPhase/n300_100_050_movegeneratorPhase.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../n450_movPhase/n450_070_movePhaseAbstract.hpp"


extern MovePhaseAbstract* g_movePhaseArray[g_MOVEGENERATOR_PHASE_NUM];
