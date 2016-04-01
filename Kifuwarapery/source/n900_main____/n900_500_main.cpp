#include "../../header/n080_common__/n080_100_common.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n240_position/n240_100_position.hpp"
#include "../../header/n240_position/n240_300_tt.hpp"
#include "../../header/n320_operate_/n320_150_search.hpp"
#include "../../header/n320_operate_/n320_200_init.hpp"
#include "../../header/n400_usi_____/n400_250_usi.hpp"
#include "../../header/n400_usi_____/n400_350_thread.hpp"
#include "..\..\header\n900_main____\n900_400_main01.h"

#if defined FIND_MAGIC
// Magic Bitboard の Magic Number を求める為のソフト
int main01() {
	u64 RookMagic[SquareNum];
	u64 BishopMagic[SquareNum];

	std::cout << "const u64 RookMagic[81] = {" << std::endl;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		RookMagic[sq] = findMagic(sq, false);
		std::cout << "\tUINT64_C(0x" << std::hex << RookMagic[sq] << ")," << std::endl;
	}
	std::cout << "};\n" << std::endl;

	std::cout << "const u64 BishopMagic[81] = {" << std::endl;
	for (Square sq = I9; sq < SquareNum; ++sq) {
		BishopMagic[sq] = findMagic(sq, true);
		std::cout << "\tUINT64_C(0x" << std::hex << BishopMagic[sq] << ")," << std::endl;
	}
	std::cout << "};\n" << std::endl;

	return 0;
}

#else
// 将棋を指すソフト
int main(int argc, char* argv[]) {
	std::unique_ptr<Searcher> searcher = std::unique_ptr<Searcher>(new Searcher);
	Main01 main01;
	main01.Initialize(searcher);
	main01.Body(argc, argv, searcher);
	main01.Finalize(searcher);
}

#endif

