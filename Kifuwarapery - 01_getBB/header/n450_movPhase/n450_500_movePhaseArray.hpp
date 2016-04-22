#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n225_movPhase/n225_050_generateMovePhase.hpp"
#include "../n440_movStack/n440_500_nextmoveEvent.hpp"
#include "../n450_movPhase/n450_070_movePhaseAbstract.hpp"


extern MovePhaseAbstract* g_movePhaseArray[GenerateMovePhase::GenerateMovePhaseNum];
