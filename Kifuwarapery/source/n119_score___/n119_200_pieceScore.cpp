﻿#include "../../header/n105_color___/n105_500_utilColor.hpp"
#include "../../header/n119_score___/n119_200_pieceScore.hpp"


const Score PieceScore::m_pawn = static_cast<Score>(100 * 9 / 10);
const Score PieceScore::m_lance = static_cast<Score>(350 * 9 / 10);
const Score PieceScore::m_knight = static_cast<Score>(450 * 9 / 10);
const Score PieceScore::m_silver = static_cast<Score>(550 * 9 / 10);
const Score PieceScore::m_gold = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_bishop = static_cast<Score>(950 * 9 / 10);
const Score PieceScore::m_rook = static_cast<Score>(1100 * 9 / 10);
const Score PieceScore::m_proPawn = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_proLance = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_proKnight = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_proSilver = static_cast<Score>(600 * 9 / 10);
const Score PieceScore::m_horse = static_cast<Score>(1050 * 9 / 10);
const Score PieceScore::m_dragon = static_cast<Score>(1550 * 9 / 10);

const Score PieceScore::m_king = static_cast<Score>(15000);

const Score PieceScore::m_capturePawn = PieceScore::m_pawn * 2;
const Score PieceScore::m_captureLance = PieceScore::m_lance * 2;
const Score PieceScore::m_captureKnight = PieceScore::m_knight * 2;
const Score PieceScore::m_captureSilver = PieceScore::m_silver * 2;
const Score PieceScore::m_captureGold = PieceScore::m_gold * 2;
const Score PieceScore::m_captureBishop = PieceScore::m_bishop * 2;
const Score PieceScore::m_captureRook = PieceScore::m_rook * 2;
const Score PieceScore::m_captureProPawn = PieceScore::m_proPawn + PieceScore::m_pawn;
const Score PieceScore::m_captureProLance = PieceScore::m_proLance + PieceScore::m_lance;
const Score PieceScore::m_captureProKnight = PieceScore::m_proKnight + PieceScore::m_knight;
const Score PieceScore::m_CaptureProSilverScore = PieceScore::m_proSilver + PieceScore::m_silver;
const Score PieceScore::m_CaptureHorseScore = PieceScore::m_horse + PieceScore::m_bishop;
const Score PieceScore::m_CaptureDragonScore = PieceScore::m_dragon + PieceScore::m_rook;
const Score PieceScore::m_CaptureKingScore = PieceScore::m_king * 2;

const Score PieceScore::m_PromotePawnScore = PieceScore::m_proPawn - PieceScore::m_pawn;
const Score PieceScore::m_PromoteLanceScore = PieceScore::m_proLance - PieceScore::m_lance;
const Score PieceScore::m_PromoteKnightScore = PieceScore::m_proKnight - PieceScore::m_knight;
const Score PieceScore::m_PromoteSilverScore = PieceScore::m_proSilver - PieceScore::m_silver;
const Score PieceScore::m_PromoteBishopScore = PieceScore::m_horse - PieceScore::m_bishop;
const Score PieceScore::m_PromoteRookScore = PieceScore::m_dragon - PieceScore::m_rook;

const Score PieceScore::m_ScoreKnownWin = PieceScore::m_king;


const Score PieceScore::m_PieceScore[Piece::N31_PieceNone] = {
	ScoreZero,
	PieceScore::m_pawn,
	PieceScore::m_lance,
	PieceScore::m_knight,
	PieceScore::m_silver,
	PieceScore::m_bishop,
	PieceScore::m_rook,
	PieceScore::m_gold,
	ScoreZero, // King
	PieceScore::m_proPawn,
	PieceScore::m_proLance,
	PieceScore::m_proKnight,
	PieceScore::m_proSilver,
	PieceScore::m_horse,
	PieceScore::m_dragon,
	ScoreZero,
	ScoreZero,
	PieceScore::m_pawn,
	PieceScore::m_lance,
	PieceScore::m_knight,
	PieceScore::m_silver,
	PieceScore::m_bishop,
	PieceScore::m_rook,
	PieceScore::m_gold,
	ScoreZero, // King
	PieceScore::m_proPawn,
	PieceScore::m_proLance,
	PieceScore::m_proKnight,
	PieceScore::m_proSilver,
	PieceScore::m_horse,
	PieceScore::m_dragon,
};
const Score PieceScore::m_CapturePieceScore[Piece::N31_PieceNone] = {
	ScoreZero,
	PieceScore::m_capturePawn,
	PieceScore::m_captureLance,
	PieceScore::m_captureKnight,
	PieceScore::m_captureSilver,
	PieceScore::m_captureBishop,
	PieceScore::m_captureRook,
	PieceScore::m_captureGold,
	ScoreZero, // King
	PieceScore::m_captureProPawn,
	PieceScore::m_captureProLance,
	PieceScore::m_captureProKnight,
	PieceScore::m_CaptureProSilverScore,
	PieceScore::m_CaptureHorseScore,
	PieceScore::m_CaptureDragonScore,
	ScoreZero,
	ScoreZero,
	PieceScore::m_capturePawn,
	PieceScore::m_captureLance,
	PieceScore::m_captureKnight,
	PieceScore::m_captureSilver,
	PieceScore::m_captureBishop,
	PieceScore::m_captureRook,
	PieceScore::m_captureGold,
	ScoreZero, // King
	PieceScore::m_captureProPawn,
	PieceScore::m_captureProLance,
	PieceScore::m_captureProKnight,
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
