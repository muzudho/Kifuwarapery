#pragma once

#include "../n113_piece___/n113_150_piece.hpp"
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n119_score___/n119_100_utilScore.hpp"


class PieceScore {
public:

	static const Score m_PawnScore = static_cast<Score>(100 * 9 / 10);
	static const Score m_LanceScore = static_cast<Score>(350 * 9 / 10);
	static const Score m_KnightScore = static_cast<Score>(450 * 9 / 10);
	static const Score m_SilverScore = static_cast<Score>(550 * 9 / 10);
	static const Score m_GoldScore = static_cast<Score>(600 * 9 / 10);
	static const Score m_BishopScore = static_cast<Score>(950 * 9 / 10);
	static const Score m_RookScore = static_cast<Score>(1100 * 9 / 10);
	static const Score m_ProPawnScore = static_cast<Score>(600 * 9 / 10);
	static const Score m_ProLanceScore = static_cast<Score>(600 * 9 / 10);
	static const Score m_ProKnightScore = static_cast<Score>(600 * 9 / 10);
	static const Score m_ProSilverScore = static_cast<Score>(600 * 9 / 10);
	static const Score m_HorseScore = static_cast<Score>(1050 * 9 / 10);
	static const Score m_DragonScore = static_cast<Score>(1550 * 9 / 10);

	static const Score m_KingScore = static_cast<Score>(15000);

	static const Score m_CapturePawnScore = PieceScore::m_PawnScore * 2;
	static const Score m_CaptureLanceScore = PieceScore::m_LanceScore * 2;
	static const Score m_CaptureKnightScore = PieceScore::m_KnightScore * 2;
	static const Score m_CaptureSilverScore = PieceScore::m_SilverScore * 2;
	static const Score m_CaptureGoldScore = PieceScore::m_GoldScore * 2;
	static const Score m_CaptureBishopScore = PieceScore::m_BishopScore * 2;
	static const Score m_CaptureRookScore = PieceScore::m_RookScore * 2;
	static const Score m_CaptureProPawnScore = PieceScore::m_ProPawnScore + PieceScore::m_PawnScore;
	static const Score m_CaptureProLanceScore = PieceScore::m_ProLanceScore + PieceScore::m_LanceScore;
	static const Score m_CaptureProKnightScore = PieceScore::m_ProKnightScore + PieceScore::m_KnightScore;
	static const Score m_CaptureProSilverScore = PieceScore::m_ProSilverScore + PieceScore::m_SilverScore;
	static const Score m_CaptureHorseScore = PieceScore::m_HorseScore + PieceScore::m_BishopScore;
	static const Score m_CaptureDragonScore = PieceScore::m_DragonScore + PieceScore::m_RookScore;
	static const Score m_CaptureKingScore = PieceScore::m_KingScore * 2;

	static const Score m_PromotePawnScore = PieceScore::m_ProPawnScore - PieceScore::m_PawnScore;
	static const Score m_PromoteLanceScore = PieceScore::m_ProLanceScore - PieceScore::m_LanceScore;
	static const Score m_PromoteKnightScore = PieceScore::m_ProKnightScore - PieceScore::m_KnightScore;
	static const Score m_PromoteSilverScore = PieceScore::m_ProSilverScore - PieceScore::m_SilverScore;
	static const Score m_PromoteBishopScore = PieceScore::m_HorseScore - PieceScore::m_BishopScore;
	static const Score m_PromoteRookScore = PieceScore::m_DragonScore - PieceScore::m_RookScore;

	static const Score m_ScoreKnownWin = PieceScore::m_KingScore;

};


extern const Score g_PieceScore[Piece::N31_PieceNone];
extern const Score g_CapturePieceScore[Piece::N31_PieceNone];
extern const Score g_PromotePieceScore[7];

