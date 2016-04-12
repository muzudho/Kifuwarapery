#include "../../header/n105_color___/n105_100_color.hpp"
#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n110_square__/n110_100_square.hpp"
#include "../../header/n112_pieceTyp/n112_050_pieceType.hpp"
#include "../../header/n160_board___/n160_100_bitboard.hpp"
#include "../../header/n170_pieceTyp/n170_110_ptPawn.hpp"
#include "../../header/n170_pieceTyp/n170_120_ptLance.hpp"
#include "../../header/n170_pieceTyp/n170_130_ptKnight.hpp"
#include "../../header/n170_pieceTyp/n170_140_ptSilver.hpp"
#include "../../header/n170_pieceTyp/n170_150_ptBishop.hpp"
#include "../../header/n170_pieceTyp/n170_160_ptRook.hpp"
#include "../../header/n170_pieceTyp/n170_170_ptGold.hpp"
#include "../../header/n170_pieceTyp/n170_180_ptKing.hpp"
#include "../../header/n220_position/n220_150_checkInfo.hpp"
#include "../../header/n220_position/n220_650_position.hpp"
#include "../../header/n276_genMove_/n276_140_makePromoteMove.hpp"	// TODO:これを外すのが難しい。

CheckInfo::CheckInfo(const Position& position) {
	const Color them = UtilColor::OppositeColor(position.GetTurn());
	const Square ksq = position.GetKingSquare(them);

	m_pinned = position.GetPinnedBB();
	m_dcBB = position.DiscoveredCheckBB();

	m_checkBB[N01_Pawn] = g_ptPawn.GetAttacks2From( g_nullBitboard, them, ksq);
	m_checkBB[N02_Lance] = g_ptLance.GetAttacks2From( position.GetOccupiedBB(), them, ksq);
	m_checkBB[N03_Knight] = g_ptKnight.GetAttacks2From( g_nullBitboard, them, ksq);
	m_checkBB[N04_Silver] = g_ptSilver.GetAttacks2From( g_nullBitboard, them, ksq);
	m_checkBB[N05_Bishop] = g_ptBishop.GetAttacks2From(position.GetOccupiedBB(), Color::ColorNum, ksq);//Colorは使ってない。
	m_checkBB[N06_Rook] = g_ptRook.GetAttacks2From(position.GetOccupiedBB(), Color::ColorNum, ksq);
	m_checkBB[N07_Gold] = g_ptGold.GetAttacks2From( g_nullBitboard, them, ksq);
	m_checkBB[N08_King] = Bitboard::CreateAllZeroBB();
	// todo: ここで AVX2 使えそう。
	//       checkBB のreadアクセスは switch (pt) で場合分けして、余計なコピー減らした方が良いかも。
	m_checkBB[N09_ProPawn] = m_checkBB[N07_Gold];
	m_checkBB[N10_ProLance] = m_checkBB[N07_Gold];
	m_checkBB[N11_ProKnight] = m_checkBB[N07_Gold];
	m_checkBB[N12_ProSilver] = m_checkBB[N07_Gold];
	m_checkBB[N13_Horse] = m_checkBB[N05_Bishop] | g_ptKing.GetAttacks2From( g_nullBitboard, Color::ColorNum, ksq);
	m_checkBB[N14_Dragon] = m_checkBB[N06_Rook] | g_ptKing.GetAttacks2From(g_nullBitboard, Color::ColorNum, ksq);
}
