#include <iostream>
#include "../../header/n110_square__/n110_150_file.hpp"
#include "../../header/n110_square__/n110_200_rank.hpp"
#include "../../header/n110_square__/n110_500_convSquare.hpp"
#include "../../header/n160_board___/n160_230_setMaskBb.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"


//────────────────────────────────────────────────────────────────────────────────
// その他
//────────────────────────────────────────────────────────────────────────────────


void PrintBb::PrintBoard(const Bitboard* thisBitboard)// const
{
	std::cout << "   A  B  C  D  E  F  G  H  I\n";
	for (Rank r = Rank9; r < RankNum; ++r) {
		std::cout << (9 - r);
		for (File f = FileA; FileI <= f; --f) {
			std::cout << (g_setMaskBb.IsSet(thisBitboard, ConvSquare::FROM_FILE_RANK10(f, r)) ? "  X" : "  .");
		}
		std::cout << "\n";
	}

	std::cout << std::endl;
}


