#pragma once

#include "../n113_piece___/n113_150_piece.hpp"
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n119_score___/n119_100_utilScore.hpp"


class PieceScore {
public:

	static const Score m_pawn;
	static const Score m_lance;
	static const Score m_knight;
	static const Score m_silver;
	static const Score m_gold;
	static const Score m_bishop;
	static const Score m_rook;
	static const Score m_proPawn;
	static const Score m_proLance;
	static const Score m_proKnight;
	static const Score m_proSilver;
	static const Score m_horse;
	static const Score m_dragon;

	static const Score m_king;

	static const Score m_capturePawn;
	static const Score m_captureLance;
	static const Score m_captureKnight;
	static const Score m_captureSilver;
	static const Score m_captureGold;
	static const Score m_captureBishop;
	static const Score m_captureRook;
	static const Score m_captureProPawn;
	static const Score m_captureProLance;
	static const Score m_captureProKnight;
	static const Score m_CaptureProSilverScore;
	static const Score m_CaptureHorseScore;
	static const Score m_CaptureDragonScore;
	static const Score m_CaptureKingScore;

	static const Score m_PromotePawnScore;
	static const Score m_PromoteLanceScore;
	static const Score m_PromoteKnightScore;
	static const Score m_PromoteSilverScore;
	static const Score m_PromoteBishopScore;
	static const Score m_PromoteRookScore;

	static const Score m_ScoreKnownWin;

	static const Score m_PieceScore[Piece::N31_PieceNone];
	static const Score m_CapturePieceScore[Piece::N31_PieceNone];
	static const Score m_PromotePieceScore[7];

public:
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	inline static const Score GetPieceScore(const PieceType pt) {
		return PieceScore::m_PieceScore[pt];
	}

	inline static const Score GetPieceScore(const Piece pc) {
		return PieceScore::m_PieceScore[pc];
	}

	inline static const Score GetCapturePieceScore(const Piece pc) {
		return PieceScore::m_CapturePieceScore[pc];
	}

	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	inline static const Score GetCapturePieceScore(const PieceType pt) {
		return PieceScore::m_CapturePieceScore[pt];
	}

	inline static const Score GetPromotePieceScore(const PieceType pt) {
		assert(pt < PieceType::N07_Gold);
		return PieceScore::m_PromotePieceScore[pt];
	}

};



