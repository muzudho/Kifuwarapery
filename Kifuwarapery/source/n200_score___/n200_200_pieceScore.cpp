#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n200_score___/n200_200_pieceScore.hpp"

const Score g_PieceScore[PieceNone] = {
	ScoreZero,
	g_PawnScore, g_LanceScore, g_KnightScore, g_SilverScore, g_BishopScore, g_RookScore, g_GoldScore,
	ScoreZero, // King
	g_ProPawnScore, g_ProLanceScore, g_ProKnightScore, g_ProSilverScore, g_HorseScore, g_DragonScore,
	ScoreZero, ScoreZero,
	g_PawnScore, g_LanceScore, g_KnightScore, g_SilverScore, g_BishopScore, g_RookScore, g_GoldScore,
	ScoreZero, // King
	g_ProPawnScore, g_ProLanceScore, g_ProKnightScore, g_ProSilverScore, g_HorseScore, g_DragonScore,
};
const Score g_CapturePieceScore[PieceNone] = {
	ScoreZero,
	g_CapturePawnScore, g_CaptureLanceScore, g_CaptureKnightScore, g_CaptureSilverScore, g_CaptureBishopScore, g_CaptureRookScore, g_CaptureGoldScore,
	ScoreZero, // King
	g_CaptureProPawnScore, g_CaptureProLanceScore, g_CaptureProKnightScore, g_CaptureProSilverScore, g_CaptureHorseScore, g_CaptureDragonScore,
	ScoreZero, ScoreZero,
	g_CapturePawnScore, g_CaptureLanceScore, g_CaptureKnightScore, g_CaptureSilverScore, g_CaptureBishopScore, g_CaptureRookScore, g_CaptureGoldScore,
	ScoreZero, // King
	g_CaptureProPawnScore, g_CaptureProLanceScore, g_CaptureProKnightScore, g_CaptureProSilverScore, g_CaptureHorseScore, g_CaptureDragonScore,
};
const Score g_PromotePieceScore[7] = {
	ScoreZero,
	g_PromotePawnScore, g_PromoteLanceScore, g_PromoteKnightScore,
	g_PromoteSilverScore, g_PromoteBishopScore, g_PromoteRookScore
};
