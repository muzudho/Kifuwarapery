﻿#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n200_score___/n200_200_pieceScore.hpp"

const Score PieceScore[PieceNone] = {
	ScoreZero,
	PawnScore, LanceScore, KnightScore, SilverScore, BishopScore, RookScore, GoldScore,
	ScoreZero, // King
	ProPawnScore, ProLanceScore, ProKnightScore, ProSilverScore, HorseScore, DragonScore,
	ScoreZero, ScoreZero,
	PawnScore, LanceScore, KnightScore, SilverScore, BishopScore, RookScore, GoldScore,
	ScoreZero, // King
	ProPawnScore, ProLanceScore, ProKnightScore, ProSilverScore, HorseScore, DragonScore,
};
const Score CapturePieceScore[PieceNone] = {
	ScoreZero,
	CapturePawnScore, CaptureLanceScore, CaptureKnightScore, CaptureSilverScore, CaptureBishopScore, CaptureRookScore, CaptureGoldScore,
	ScoreZero, // King
	CaptureProPawnScore, CaptureProLanceScore, CaptureProKnightScore, CaptureProSilverScore, CaptureHorseScore, CaptureDragonScore,
	ScoreZero, ScoreZero,
	CapturePawnScore, CaptureLanceScore, CaptureKnightScore, CaptureSilverScore, CaptureBishopScore, CaptureRookScore, CaptureGoldScore,
	ScoreZero, // King
	CaptureProPawnScore, CaptureProLanceScore, CaptureProKnightScore, CaptureProSilverScore, CaptureHorseScore, CaptureDragonScore,
};
const Score PromotePieceScore[7] = {
	ScoreZero,
	PromotePawnScore, PromoteLanceScore, PromoteKnightScore,
	PromoteSilverScore, PromoteBishopScore, PromoteRookScore
};
