#include <iostream>
#include "../../header/n160_board___/n160_400_printBb.hpp"


//����������������������������������������������������������������������������������������������������������������������������������������������������������������
// ���̑�
//����������������������������������������������������������������������������������������������������������������������������������������������������������������


void PrintBb::PrintBoard(const Bitboard* thisBitboard)// const
{
	std::cout << "   A  B  C  D  E  F  G  H  I\n";
	for (Rank r = Rank9; r < RankNum; ++r) {
		std::cout << (9 - r);
		for (File f = FileA; FileI <= f; --f) {
			std::cout << (g_setMaskBb.IsSet(thisBitboard, UtilSquare::FromFileRank(f, r)) ? "  X" : "  .");
		}
		std::cout << "\n";
	}

	std::cout << std::endl;
}


