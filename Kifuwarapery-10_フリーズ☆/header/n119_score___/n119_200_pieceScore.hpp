#pragma once

#include "../n113_piece___/n113_150_piece.hpp"
#include "../n113_piece___/n113_205_convHandPiece.hpp"
#include "n119_090_scoreIndex.hpp"
#include "n119_100_utilScore.hpp"


class PieceScore {
public:

	static const ScoreIndex m_pawn;
	static const ScoreIndex m_lance;
	static const ScoreIndex m_knight;
	static const ScoreIndex m_silver;
	static const ScoreIndex m_gold;
	static const ScoreIndex m_bishop;
	static const ScoreIndex m_rook;
	static const ScoreIndex m_proPawn;
	static const ScoreIndex m_proLance;
	static const ScoreIndex m_proKnight;
	static const ScoreIndex m_proSilver;
	static const ScoreIndex m_horse;
	static const ScoreIndex m_dragon;

	static const ScoreIndex m_king;

	static const ScoreIndex m_capturePawn;
	static const ScoreIndex m_captureLance;
	static const ScoreIndex m_captureKnight;
	static const ScoreIndex m_captureSilver;
	static const ScoreIndex m_captureGold;
	static const ScoreIndex m_captureBishop;
	static const ScoreIndex m_captureRook;
	static const ScoreIndex m_captureProPawn;
	static const ScoreIndex m_captureProLance;
	static const ScoreIndex m_captureProKnight;
	static const ScoreIndex m_CaptureProSilverScore;
	static const ScoreIndex m_CaptureHorseScore;
	static const ScoreIndex m_CaptureDragonScore;
	static const ScoreIndex m_CaptureKingScore;

	static const ScoreIndex m_PromotePawnScore;
	static const ScoreIndex m_PromoteLanceScore;
	static const ScoreIndex m_PromoteKnightScore;
	static const ScoreIndex m_PromoteSilverScore;
	static const ScoreIndex m_PromoteBishopScore;
	static const ScoreIndex m_PromoteRookScore;

	static const ScoreIndex m_ScoreKnownWin;

	static const ScoreIndex m_PieceScore[Piece::N31_PieceNone];
	static const ScoreIndex m_CapturePieceScore[Piece::N31_PieceNone];
	static const ScoreIndex m_PromotePieceScore[7];

public:
	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	inline static const ScoreIndex GetPieceScore(const PieceType pt) {
		return PieceScore::m_PieceScore[pt];
	}

	inline static const ScoreIndex GetPieceScore(const Piece pc) {
		return PieceScore::m_PieceScore[pc];
	}

	inline static const ScoreIndex GetCapturePieceScore(const Piece pc) {
		return PieceScore::m_CapturePieceScore[pc];
	}

	// Piece を index としても、 PieceType を index としても、
	// 同じ値が取得出来るようにしているので、PieceType => Piece への変換は必要ない。
	inline static const ScoreIndex GetCapturePieceScore(const PieceType pt) {
		return PieceScore::m_CapturePieceScore[pt];
	}

	inline static const ScoreIndex GetPromotePieceScore(const PieceType pt) {
		assert(pt < PieceType::N07_Gold);
		return PieceScore::m_PromotePieceScore[pt];
	}

};



