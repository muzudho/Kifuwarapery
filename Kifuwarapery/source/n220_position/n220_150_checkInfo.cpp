#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n113_piece___/n113_100_pieceType.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n220_position/n220_150_checkInfo.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n276_genMove_/n276_140_makePromoteMove.hpp"	// TODO:これを外すのが難しい。

CheckInfo::CheckInfo(const Position& position) {
	const Color them = UtilColor::OppositeColor(position.GetTurn());
	const Square ksq = position.GetKingSquare(them);

	m_pinned = position.GetPinnedBB();
	m_dcBB = position.DiscoveredCheckBB();

	m_checkBB[Pawn] = position.GetAttacksFrom<Pawn  >(them, ksq);
	m_checkBB[Lance] = position.GetAttacksFrom<Lance >(them, ksq);
	m_checkBB[Knight] = position.GetAttacksFrom<Knight>(them, ksq);
	m_checkBB[Silver] = position.GetAttacksFrom<Silver>(them, ksq);
	m_checkBB[Bishop] = position.GetAttacksFrom<Bishop>(ksq);
	m_checkBB[Rook] = position.GetAttacksFrom<Rook  >(ksq);
	m_checkBB[Gold] = position.GetAttacksFrom<Gold  >(them, ksq);
	m_checkBB[King] = Bitboard::CreateAllZeroBB();
	// todo: ここで AVX2 使えそう。
	//       checkBB のreadアクセスは switch (pt) で場合分けして、余計なコピー減らした方が良いかも。
	m_checkBB[ProPawn] = m_checkBB[Gold];
	m_checkBB[ProLance] = m_checkBB[Gold];
	m_checkBB[ProKnight] = m_checkBB[Gold];
	m_checkBB[ProSilver] = m_checkBB[Gold];
	m_checkBB[Horse] = m_checkBB[Bishop] | position.GetAttacksFrom<King>(ksq);
	m_checkBB[Dragon] = m_checkBB[Rook] | position.GetAttacksFrom<King>(ksq);
}
