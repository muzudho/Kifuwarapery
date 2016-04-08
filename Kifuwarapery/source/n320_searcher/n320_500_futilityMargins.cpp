#include "../../header/n320_searcher/n320_500_futilityMargins.hpp"


//Score g_FutilityMargins[16][64]; // [depth][moveCount]
FutilityMargins g_futilityMargins;

void FutilityMargins::Initialize()
{
	//int iDepth;  // depth (ONE_PLY == 2)
	//int iMoveCount; // moveCount
	for (int iDepth = 1; iDepth < 16; ++iDepth) {
		for (int iMoveCount = 0; iMoveCount < 64; ++iMoveCount) {
			this->m_FutilityMargins[iDepth][iMoveCount] =
				static_cast<Score>(
					112 * static_cast<int>(
						log(
							static_cast<double>(iDepth*iDepth) / 2
						)
						/
						log(2.0)
						+
						1.001
					)
					- 8 * iMoveCount + 45
				);
		}
	}
}
