#include "../../header/n640_searcher/n640_500_reductions.hpp"


Reductions g_reductions;


void Reductions::Initialize()
{
	// Init reductions array
	//int iHalfDepth; // half depth (ONE_PLY == 1)
	//int iMoveCount; // moveCount
	for (int iHalfDepth = 1; iHalfDepth < 64; iHalfDepth++) {
		for (int iMoveCount = 1; iMoveCount < 64; iMoveCount++) {
			double    pvRed = log(double(iHalfDepth)) * log(double(iMoveCount)) / 3.0;
			double nonPVRed = 0.33 + log(double(iHalfDepth)) * log(double(iMoveCount)) / 2.25;
			g_reductions.m_reductions[1][iHalfDepth][iMoveCount] = (int8_t)(pvRed >= 1.0 ? floor(pvRed * int(OnePly)) : 0);
			g_reductions.m_reductions[0][iHalfDepth][iMoveCount] = (int8_t)(nonPVRed >= 1.0 ? floor(nonPVRed * int(OnePly)) : 0);
		}
	}
}
