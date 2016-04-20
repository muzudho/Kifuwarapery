#pragma once

#include "../n113_piece___/n113_150_piece.hpp"
#include "../n113_piece___/n113_205_utilHandPiece.hpp"
#include "../n119_score___/n119_100_utilScore.hpp"


class PieceScore {
public:

	static const Score m_PawnScore;
	static const Score m_LanceScore;
	static const Score m_KnightScore;
	static const Score m_SilverScore;
	static const Score m_GoldScore;
	static const Score m_BishopScore;
	static const Score m_RookScore;
	static const Score m_ProPawnScore;
	static const Score m_ProLanceScore;
	static const Score m_ProKnightScore;
	static const Score m_ProSilverScore;
	static const Score m_HorseScore;
	static const Score m_DragonScore;

	static const Score m_KingScore;

	static const Score m_CapturePawnScore;
	static const Score m_CaptureLanceScore;
	static const Score m_CaptureKnightScore;
	static const Score m_CaptureSilverScore;
	static const Score m_CaptureGoldScore;
	static const Score m_CaptureBishopScore;
	static const Score m_CaptureRookScore;
	static const Score m_CaptureProPawnScore;
	static const Score m_CaptureProLanceScore;
	static const Score m_CaptureProKnightScore;
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



