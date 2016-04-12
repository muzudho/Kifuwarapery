#include "../../header/n220_position/n220_400_evalList.hpp"
#include "../../header/n220_position/n220_750_charToPieceUSI.hpp"
#include "../../header/n260_evaluate/n260_500_kkKkpKppStorage1.hpp"
#include "../../header/n260_evaluate/n260_700_evaluation09.hpp"

const Square g_HandPieceToSquareHand[ColorNum][HandPieceNum] = {
	{B_hand_pawn, B_hand_lance, B_hand_knight, B_hand_silver, B_hand_gold, B_hand_bishop, B_hand_rook},
	{W_hand_pawn, W_hand_lance, W_hand_knight, W_hand_silver, W_hand_gold, W_hand_bishop, W_hand_rook}
};

void EvalList::Set(const Position& pos) {
	const Hand handB = pos.GetHand(Black);
	const Hand handW = pos.GetHand(White);

	int nlist = 0;
	auto func = [&nlist, this](const Hand hand, const HandPiece hp, const int list0_index, const int list1_index, const Color c) {
		for (u32 i = 1; i <= hand.NumOf(hp); ++i) {
			m_list0[nlist] = list0_index + i;
			m_list1[nlist] = list1_index + i;
			const Square squarehand = g_HandPieceToSquareHand[c][hp] + static_cast<Square>(i);
			m_listToSquareHand[nlist] = squarehand;
			m_squareHandToList[squarehand] = nlist;
			++nlist;
		}
	};
	func(handB, HPawn  , f_hand_pawn  , e_hand_pawn  , Black);
	func(handW, HPawn  , e_hand_pawn  , f_hand_pawn  , White);
	func(handB, HLance , f_hand_lance , e_hand_lance , Black);
	func(handW, HLance , e_hand_lance , f_hand_lance , White);
	func(handB, HKnight, f_hand_knight, e_hand_knight, Black);
	func(handW, HKnight, e_hand_knight, f_hand_knight, White);
	func(handB, HSilver, f_hand_silver, e_hand_silver, Black);
	func(handW, HSilver, e_hand_silver, f_hand_silver, White);
	func(handB, HGold  , f_hand_gold  , e_hand_gold  , Black);
	func(handW, HGold  , e_hand_gold  , f_hand_gold  , White);
	func(handB, HBishop, f_hand_bishop, e_hand_bishop, Black);
	func(handW, HBishop, e_hand_bishop, f_hand_bishop, White);
	func(handB, HRook  , f_hand_rook  , e_hand_rook  , Black);
	func(handW, HRook  , e_hand_rook  , f_hand_rook  , White);

	Bitboard bb = pos.GetBbOf(King).NotThisAnd(pos.GetOccupiedBB());
	while (bb.Exists1Bit()) {
		const Square sq = bb.PopFirstOneFromI9();
		const Piece pc = pos.GetPiece(sq);
		m_listToSquareHand[nlist] = sq;
		m_squareHandToList[sq] = nlist;
		m_list0[nlist  ] = kppArray[pc         ] + sq;
		m_list1[nlist++] = kppArray[UtilPiece::Inverse(pc)] + UtilSquare::Inverse(sq);
	}
}
