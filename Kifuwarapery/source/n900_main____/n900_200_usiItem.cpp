#include <iostream>
#include "../../header/n300_moveGen_/n300_200_pieceTyp/n300_200_020_moveStack.hpp"
#include "../../header/n300_moveGen_/n300_700_moveGen_/n300_700_800_moveGenerator200.hpp"
#include "../../header/n800_stpWatch/n800_100_stopwatch.hpp"
#include "../../header/n900_main____/n900_200_usiItem.hpp"


#if !defined MINIMUL
// for debug
// 指し手生成の速度を計測
void UsiItem::MeasureGenerateMoves(const Position& pos) {
	pos.Print();

	MoveStack legalMoves[Move::m_MAX_LEGAL_MOVES];
	for (int i = 0; i < Move::m_MAX_LEGAL_MOVES; ++i)
	{
		legalMoves[i].m_move = g_MOVE_NONE;
	}
	MoveStack* pms = &legalMoves[0];
	const u64 num = 5000000;
	Stopwatch t = Stopwatch::CreateStopwatchByCurrentTime();
	if (pos.InCheck()) {
		for (u64 i = 0; i < num; ++i) {
			pms = &legalMoves[0];
			pms = g_moveGenerator200.GenerateMoves_2(N06_Evasion, pms, pos);
		}
	}
	else {
		for (u64 i = 0; i < num; ++i) {
			pms = &legalMoves[0];
			pms = g_moveGenerator200.GenerateMoves_2(N03_CapturePlusPro, pms, pos);
			pms = g_moveGenerator200.GenerateMoves_2(N04_NonCaptureMinusPro, pms, pos);
			pms = g_moveGenerator200.GenerateMoves_2(N02_Drop, pms, pos);
			//			pms = generateMoves<PseudoLegal>(pms, pos);
			//			pms = generateMoves<Legal>(pms, pos);
		}
	}
	const int elapsed = t.GetElapsed();
	std::cout << "elapsed = " << elapsed << " [msec]" << std::endl;
	if (elapsed != 0) {
		std::cout << "times/s = " << num * 1000 / elapsed << " [times/sec]" << std::endl;
	}
	const ptrdiff_t count = pms - &legalMoves[0];
	std::cout << "num of moves = " << count << std::endl;
	for (int i = 0; i < count; ++i) {
		std::cout << legalMoves[i].m_move.ToCSA() << ", ";
	}
	std::cout << std::endl;
}
#endif

