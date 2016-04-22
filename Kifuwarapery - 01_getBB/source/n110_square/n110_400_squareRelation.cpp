#include "../../header/n110_square__/n110_400_squareRelation.hpp"
#include "../../header/n110_square__/n110_500_convSquare.hpp"


SquareRelation g_squareRelation;


void SquareRelation::Initialize()
{
	for (Square sq1 = I9; sq1 < SquareNum; ++sq1) {
		const File file1 = ConvSquare::TO_FILE10(sq1);
		const Rank rank1 = ConvSquare::TO_RANK10(sq1);
		for (Square sq2 = I9; sq2 < SquareNum; ++sq2) {
			const File file2 = ConvSquare::TO_FILE10(sq2);
			const Rank rank2 = ConvSquare::TO_RANK10(sq2);
			this->m_squareRelation_[sq1][sq2] = N00_DirecMisc;
			if (sq1 == sq2) continue;

			if (file1 == file2)
				this->m_squareRelation_[sq1][sq2] = N02_DirecFile;
			else if (rank1 == rank2)
				this->m_squareRelation_[sq1][sq2] = N03_DirecRank;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file1 - file2))
				this->m_squareRelation_[sq1][sq2] = N04_DirecDiagNESW;
			else if (static_cast<int>(rank1 - rank2) == static_cast<int>(file2 - file1))
				this->m_squareRelation_[sq1][sq2] = N05_DirecDiagNWSE;
		}
	}
}
