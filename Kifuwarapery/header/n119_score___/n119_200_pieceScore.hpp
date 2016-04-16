#pragma once

#include "../n113_piece___/n113_150_piece.hpp"
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n119_score___/n119_100_utilScore.hpp"

const Score g_PawnScore             = static_cast<Score>( 100 * 9 / 10);
const Score g_LanceScore            = static_cast<Score>( 350 * 9 / 10);
const Score g_KnightScore           = static_cast<Score>( 450 * 9 / 10);
const Score g_SilverScore           = static_cast<Score>( 550 * 9 / 10);
const Score g_GoldScore             = static_cast<Score>( 600 * 9 / 10);
const Score g_BishopScore           = static_cast<Score>( 950 * 9 / 10);
const Score g_RookScore             = static_cast<Score>(1100 * 9 / 10);
const Score g_ProPawnScore          = static_cast<Score>( 600 * 9 / 10);
const Score g_ProLanceScore         = static_cast<Score>( 600 * 9 / 10);
const Score g_ProKnightScore        = static_cast<Score>( 600 * 9 / 10);
const Score g_ProSilverScore        = static_cast<Score>( 600 * 9 / 10);
const Score g_HorseScore            = static_cast<Score>(1050 * 9 / 10);
const Score g_DragonScore           = static_cast<Score>(1550 * 9 / 10);

const Score g_KingScore             = static_cast<Score>(15000);

const Score g_CapturePawnScore      = g_PawnScore      * 2;
const Score g_CaptureLanceScore     = g_LanceScore     * 2;
const Score g_CaptureKnightScore    = g_KnightScore    * 2;
const Score g_CaptureSilverScore    = g_SilverScore    * 2;
const Score g_CaptureGoldScore      = g_GoldScore      * 2;
const Score g_CaptureBishopScore    = g_BishopScore    * 2;
const Score g_CaptureRookScore      = g_RookScore      * 2;
const Score g_CaptureProPawnScore   = g_ProPawnScore   + g_PawnScore;
const Score g_CaptureProLanceScore  = g_ProLanceScore  + g_LanceScore;
const Score g_CaptureProKnightScore = g_ProKnightScore + g_KnightScore;
const Score g_CaptureProSilverScore = g_ProSilverScore + g_SilverScore;
const Score g_CaptureHorseScore     = g_HorseScore     + g_BishopScore;
const Score g_CaptureDragonScore    = g_DragonScore    + g_RookScore;
const Score g_CaptureKingScore      = g_KingScore      * 2;

const Score g_PromotePawnScore      = g_ProPawnScore   - g_PawnScore;
const Score g_PromoteLanceScore     = g_ProLanceScore  - g_LanceScore;
const Score g_PromoteKnightScore    = g_ProKnightScore - g_KnightScore;
const Score g_PromoteSilverScore    = g_ProSilverScore - g_SilverScore;
const Score g_PromoteBishopScore    = g_HorseScore     - g_BishopScore;
const Score g_PromoteRookScore      = g_DragonScore    - g_RookScore;

const Score g_ScoreKnownWin = g_KingScore;

extern const Score g_PieceScore[N31_PieceNone];
extern const Score g_CapturePieceScore[N31_PieceNone];
extern const Score g_PromotePieceScore[7];

