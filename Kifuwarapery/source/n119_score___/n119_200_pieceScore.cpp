#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n119_score___/n119_200_pieceScore.hpp"


const Score PieceScore::m_PawnScore = static_cast<Score>(100 * 9 / 10);
const Score PieceScore::m_LanceScore = static_cast<Score>(350 * 9 / 10);
const Score PieceScore::m_KnightScore = static_cast<Score>(450 * 9 / 10);
const Score PieceScore::m_SilverScore = static_cast<Score>(550 * 9 / 10);
const Score PieceScore::m_GoldScore = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_BishopScore = static_cast<Score>(950 * 9 / 10);
const Score PieceScore::m_RookScore = static_cast<Score>(1100 * 9 / 10);
const Score PieceScore::m_ProPawnScore = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_ProLanceScore = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_ProKnightScore = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_ProSilverScore = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_HorseScore = static_cast<Score>(1050 * 9 / 10);
const Score PieceScore::m_DragonScore = static_cast<Score>(1550 * 9 / 10);

const Score PieceScore::m_KingScore = static_cast<Score>(15000);

const Score PieceScore::m_CapturePawnScore = PieceScore::m_PawnScore * 2;
const Score PieceScore::m_CaptureLanceScore = PieceScore::m_LanceScore * 2;
const Score PieceScore::m_CaptureKnightScore = PieceScore::m_KnightScore * 2;
const Score PieceScore::m_CaptureSilverScore = PieceScore::m_SilverScore * 2;
const Score PieceScore::m_CaptureGoldScore = PieceScore::m_GoldScore * 2;
const Score PieceScore::m_CaptureBishopScore = PieceScore::m_BishopScore * 2;
const Score PieceScore::m_CaptureRookScore = PieceScore::m_RookScore * 2;
const Score PieceScore::m_CaptureProPawnScore = PieceScore::m_ProPawnScore + PieceScore::m_PawnScore;
const Score PieceScore::m_CaptureProLanceScore = PieceScore::m_ProLanceScore + PieceScore::m_LanceScore;
const Score PieceScore::m_CaptureProKnightScore = PieceScore::m_ProKnightScore + PieceScore::m_KnightScore;
const Score PieceScore::m_CaptureProSilverScore = PieceScore::m_ProSilverScore + PieceScore::m_SilverScore;
const Score PieceScore::m_CaptureHorseScore = PieceScore::m_HorseScore + PieceScore::m_BishopScore;
const Score PieceScore::m_CaptureDragonScore = PieceScore::m_DragonScore + PieceScore::m_RookScore;
const Score PieceScore::m_CaptureKingScore = PieceScore::m_KingScore * 2;

const Score PieceScore::m_PromotePawnScore = PieceScore::m_ProPawnScore - PieceScore::m_PawnScore;
const Score PieceScore::m_PromoteLanceScore = PieceScore::m_ProLanceScore - PieceScore::m_LanceScore;
const Score PieceScore::m_PromoteKnightScore = PieceScore::m_ProKnightScore - PieceScore::m_KnightScore;
const Score PieceScore::m_PromoteSilverScore = PieceScore::m_ProSilverScore - PieceScore::m_SilverScore;
const Score PieceScore::m_PromoteBishopScore = PieceScore::m_HorseScore - PieceScore::m_BishopScore;
const Score PieceScore::m_PromoteRookScore = PieceScore::m_DragonScore - PieceScore::m_RookScore;

const Score PieceScore::m_ScoreKnownWin = PieceScore::m_KingScore;


const Score PieceScore::m_PieceScore[Piece::N31_PieceNone] = {
	ScoreZero,
	PieceScore::m_PawnScore,
	PieceScore::m_LanceScore,
	PieceScore::m_KnightScore,
	PieceScore::m_SilverScore,
	PieceScore::m_BishopScore,
	PieceScore::m_RookScore,
	PieceScore::m_GoldScore,
	ScoreZero, // King
	PieceScore::m_ProPawnScore,
	PieceScore::m_ProLanceScore,
	PieceScore::m_ProKnightScore,
	PieceScore::m_ProSilverScore,
	PieceScore::m_HorseScore,
	PieceScore::m_DragonScore,
	ScoreZero,
	ScoreZero,
	PieceScore::m_PawnScore,
	PieceScore::m_LanceScore,
	PieceScore::m_KnightScore,
	PieceScore::m_SilverScore,
	PieceScore::m_BishopScore,
	PieceScore::m_RookScore,
	PieceScore::m_GoldScore,
	ScoreZero, // King
	PieceScore::m_ProPawnScore,
	PieceScore::m_ProLanceScore,
	PieceScore::m_ProKnightScore,
	PieceScore::m_ProSilverScore,
	PieceScore::m_HorseScore,
	PieceScore::m_DragonScore,
};
const Score PieceScore::m_CapturePieceScore[Piece::N31_PieceNone] = {
	ScoreZero,
	PieceScore::m_CapturePawnScore,
	PieceScore::m_CaptureLanceScore,
	PieceScore::m_CaptureKnightScore,
	PieceScore::m_CaptureSilverScore,
	PieceScore::m_CaptureBishopScore,
	PieceScore::m_CaptureRookScore,
	PieceScore::m_CaptureGoldScore,
	ScoreZero, // King
	PieceScore::m_CaptureProPawnScore,
	PieceScore::m_CaptureProLanceScore,
	PieceScore::m_CaptureProKnightScore,
	PieceScore::m_CaptureProSilverScore,
	PieceScore::m_CaptureHorseScore,
	PieceScore::m_CaptureDragonScore,
	ScoreZero,
	ScoreZero,
	PieceScore::m_CapturePawnScore,
	PieceScore::m_CaptureLanceScore,
	PieceScore::m_CaptureKnightScore,
	PieceScore::m_CaptureSilverScore,
	PieceScore::m_CaptureBishopScore,
	PieceScore::m_CaptureRookScore,
	PieceScore::m_CaptureGoldScore,
	ScoreZero, // King
	PieceScore::m_CaptureProPawnScore,
	PieceScore::m_CaptureProLanceScore,
	PieceScore::m_CaptureProKnightScore,
	PieceScore::m_CaptureProSilverScore,
	PieceScore::m_CaptureHorseScore,
	PieceScore::m_CaptureDragonScore,
};
const Score PieceScore::m_PromotePieceScore[7] = {
	ScoreZero,
	PieceScore::m_PromotePawnScore,
	PieceScore::m_PromoteLanceScore,
	PieceScore::m_PromoteKnightScore,
	PieceScore::m_PromoteSilverScore,
	PieceScore::m_PromoteBishopScore,
	PieceScore::m_PromoteRookScore
};
