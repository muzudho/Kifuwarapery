#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n220_position/n220_150_checkInfo.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n276_genMove_/n276_140_makePromoteMove.hpp"	// TODO:これを外すのが難しい。

CheckInfo::CheckInfo(const Position& position) {
	const Color them = UtilColor::OppositeColor(position.GetTurn());
	const Square ksq = position.GetKingSquare(them);

	m_pinned = position.GetPinnedBB();
	m_dcBB = position.DiscoveredCheckBB();

	m_checkBB[N01_Pawn] = position.GetAttacksFrom<N01_Pawn  >(them, ksq);
	m_checkBB[N02_Lance] = position.GetAttacksFrom<N02_Lance >(them, ksq);
	m_checkBB[N03_Knight] = position.GetAttacksFrom<N03_Knight>(them, ksq);
	m_checkBB[N04_Silver] = position.GetAttacksFrom<N04_Silver>(them, ksq);
	m_checkBB[N05_Bishop] = position.GetAttacksFrom<N05_Bishop>(ksq);
	m_checkBB[N06_Rook] = position.GetAttacksFrom<N06_Rook  >(ksq);
	m_checkBB[N07_Gold] = position.GetAttacksFrom<N07_Gold  >(them, ksq);
	m_checkBB[N08_King] = Bitboard::CreateAllZeroBB();
	// todo: ここで AVX2 使えそう。
	//       checkBB のreadアクセスは switch (pt) で場合分けして、余計なコピー減らした方が良いかも。
	m_checkBB[N09_ProPawn] = m_checkBB[N07_Gold];
	m_checkBB[N10_ProLance] = m_checkBB[N07_Gold];
	m_checkBB[N11_ProKnight] = m_checkBB[N07_Gold];
	m_checkBB[N12_ProSilver] = m_checkBB[N07_Gold];
	m_checkBB[N13_Horse] = m_checkBB[N05_Bishop] | position.GetAttacksFrom<N08_King>(ksq);
	m_checkBB[N14_Dragon] = m_checkBB[N06_Rook] | position.GetAttacksFrom<N08_King>(ksq);
}
