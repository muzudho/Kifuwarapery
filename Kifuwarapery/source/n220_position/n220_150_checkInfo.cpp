#include <sstream>      // std::istringstream
#include "../../header/n160_board___/n160_250_squareRelation.hpp"
#include "../../header/n160_board___/n160_400_printBb.hpp"
#include "../../header/n160_board___/n160_106_inFrontMaskBb.hpp"
#include "../../header/n220_position/n220_150_checkInfo.hpp"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n223_move____/n223_105_utilMove.hpp"
#include "../../header/n240_tt______/n240_300_tt.hpp"
#include "../../header/n276_genMove_/n276_250_makePromoteMove.hpp"
#include "../../header/n280_move____/n280_200_mt64bit.hpp"
#include "../../header/n320_searcher/n320_150_search.hpp"


CheckInfo::CheckInfo(const Position& pos) {
	const Color them = UtilColor::OppositeColor(pos.GetTurn());
	const Square ksq = pos.GetKingSquare(them);

	m_pinned = pos.GetPinnedBB();
	m_dcBB = pos.DiscoveredCheckBB();

	m_checkBB[Pawn] = pos.GetAttacksFrom<Pawn  >(them, ksq);
	m_checkBB[Lance] = pos.GetAttacksFrom<Lance >(them, ksq);
	m_checkBB[Knight] = pos.GetAttacksFrom<Knight>(them, ksq);
	m_checkBB[Silver] = pos.GetAttacksFrom<Silver>(them, ksq);
	m_checkBB[Bishop] = pos.GetAttacksFrom<Bishop>(ksq);
	m_checkBB[Rook] = pos.GetAttacksFrom<Rook  >(ksq);
	m_checkBB[Gold] = pos.GetAttacksFrom<Gold  >(them, ksq);
	m_checkBB[King] = Bitboard::CreateAllZeroBB();
	// todo: ここで AVX2 使えそう。
	//       checkBB のreadアクセスは switch (pt) で場合分けして、余計なコピー減らした方が良いかも。
	m_checkBB[ProPawn] = m_checkBB[Gold];
	m_checkBB[ProLance] = m_checkBB[Gold];
	m_checkBB[ProKnight] = m_checkBB[Gold];
	m_checkBB[ProSilver] = m_checkBB[Gold];
	m_checkBB[Horse] = m_checkBB[Bishop] | pos.GetAttacksFrom<King>(ksq);
	m_checkBB[Dragon] = m_checkBB[Rook] | pos.GetAttacksFrom<King>(ksq);
}
