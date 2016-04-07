#include "../../header/n110_square__/n110_500_utilSquare.hpp"
#include "../../header/n160_board___/n160_250_squareRelation.hpp"


SquareRelation g_squareRelation;


void SquareRelation::Initialize()
{
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		const File file1 = UtilSquare::ToFile(sq1);
		const Rank rank1 = UtilSquare::ToRank(sq1);
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			const File file2 = UtilSquare::ToFile(sq2);
			const Rank rank2 = UtilSquare::ToRank(sq2);
			this->m_squareRelation[sq1][sq2] = DirecMisc;
			if (sq1 == sq2) continue;

			if (file1 == file2)
				this->m_squareRelation[sq1][sq2] = DirecFile;
			else if (rank1 == rank2)
				this->m_squareRelation[sq1][sq2] = DirecRank;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file1 - file2))
				this->m_squareRelation[sq1][sq2] = DirecDiagNESW;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file2 - file1))
				this->m_squareRelation[sq1][sq2] = DirecDiagNWSE;
		}
	}
}
