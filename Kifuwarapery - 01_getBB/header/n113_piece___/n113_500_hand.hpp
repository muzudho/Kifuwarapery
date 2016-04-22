#pragma once

#include "../n080_common__/n080_100_common.hpp"
#include "../n113_piece___/n113_200_handPiece.hpp"

// 手駒
// 手駒の状態 (32bit に pack する)
// 手駒の優劣判定を高速に行う為に各駒の間を1bit空ける。
// xxxxxxxx xxxxxxxx xxxxxxxx xxx11111  Pawn
// xxxxxxxx xxxxxxxx xxxxxxx1 11xxxxxx  Lance
// xxxxxxxx xxxxxxxx xxx111xx xxxxxxxx  Knight
// xxxxxxxx xxxxxxx1 11xxxxxx xxxxxxxx  Silver
// xxxxxxxx xxx111xx xxxxxxxx xxxxxxxx  Gold
// xxxxxxxx 11xxxxxx xxxxxxxx xxxxxxxx  Bishop
// xxxxx11x xxxxxxxx xxxxxxxx xxxxxxxx  Rook
class Hand {
public:

	Hand() {}

	explicit Hand(u32 v) : m_value_(v) {}

	u32 Value() const;

	template <HandPiece HP>
	u32 NumOf() const {
		return (HP == HPawn ? ((Value() & m_HPawnMask) >> m_HPawnShiftBits) :
			HP == HLance ? ((Value() & m_HLanceMask) >> m_HLanceShiftBits) :
			HP == HKnight ? ((Value() & m_HKnightMask) >> m_HKnightShiftBits) :
			HP == HSilver ? ((Value() & m_HSilverMask) >> m_HSilverShiftBits) :
			HP == HGold ? ((Value() & m_HGoldMask) >> m_HGoldShiftBits) :
			HP == HBishop ? ((Value() & m_HBishopMask) >> m_HBishopShiftBits) :
			/*HP == HRook   ?*/ ((Value() /*& HRookMask*/) >> m_HRookShiftBits));
	}

	u32 NumOf(const HandPiece handPiece) const;

	// 2つの Hand 型変数の、同じ種類の駒の数を比較する必要があるため、
	// bool じゃなくて、u32 型でそのまま返す。
	static u32 Exists_HPawn(const Hand& hand) { return hand.Value() & m_HPawnMask; }
	static u32 Exists_HLance(const Hand& hand) { return hand.Value() & m_HLanceMask; }
	static u32 Exists_HKnight(const Hand& hand) { return hand.Value() & m_HKnightMask; }
	static u32 Exists_HSilver(const Hand& hand) { return hand.Value() & m_HSilverMask; }
	static u32 Exists_HGold(const Hand& hand) { return hand.Value() & m_HGoldMask; }
	static u32 Exists_HBishop(const Hand& hand) { return hand.Value() & m_HBishopMask; }
	static u32 Exists_HRook(const Hand& hand) { return hand.Value() & m_HRookMask; }

	u32 Exists(const HandPiece handPiece) const;

	static u32 ExceptPawnExists(const Hand& hand);

	// num が int だけどまあ良いか。
	void OrEqual(const int num, const HandPiece handPiece);

	void PlusOne(const HandPiece handPiece);

	void MinusOne(const HandPiece handPiece);

	bool operator == (const Hand rhs) const;

	bool operator != (const Hand rhs) const;

	// 手駒の優劣判定
	// 手駒が ref と同じか、勝っていれば true
	// 勝っている状態とは、全ての種類の手駒が、ref 以上の枚数があることを言う。
	bool IsEqualOrSuperior(const Hand ref) const;

private:
	static const int m_HPawnShiftBits   =  0;
	static const int m_HLanceShiftBits  =  6;
	static const int m_HKnightShiftBits = 10;
	static const int m_HSilverShiftBits = 14;
	static const int m_HGoldShiftBits   = 18;
	static const int m_HBishopShiftBits = 22;
	static const int m_HRookShiftBits   = 25;
	static const u32 m_HPawnMask   = 0x1f << m_HPawnShiftBits;
	static const u32 m_HLanceMask  = 0x7  << m_HLanceShiftBits;
	static const u32 m_HKnightMask = 0x7  << m_HKnightShiftBits;
	static const u32 m_HSilverMask = 0x7  << m_HSilverShiftBits;
	static const u32 m_HGoldMask   = 0x7  << m_HGoldShiftBits;
	static const u32 m_HBishopMask = 0x3  << m_HBishopShiftBits;
	static const u32 m_HRookMask   = 0x3  << m_HRookShiftBits;
	static const u32 m_HandPieceExceptPawnMask = (m_HLanceMask  | m_HKnightMask |
												m_HSilverMask | m_HGoldMask   |
												m_HBishopMask | m_HRookMask  );
	static const int m_HandPieceShiftBits[HandPieceNum];
	static const u32 m_HandPieceMask[HandPieceNum];
	// 特定の種類の持ち駒を 1 つ増やしたり減らしたりするときに使用するテーブル
	static const u32 m_HandPieceOne[HandPieceNum];
	static const u32 m_BorrowMask = ((m_HPawnMask   + (1 << m_HPawnShiftBits  )) | 
								   (m_HLanceMask  + (1 << m_HLanceShiftBits )) | 
								   (m_HKnightMask + (1 << m_HKnightShiftBits)) | 
								   (m_HSilverMask + (1 << m_HSilverShiftBits)) | 
								   (m_HGoldMask   + (1 << m_HGoldShiftBits  )) | 
								   (m_HBishopMask + (1 << m_HBishopShiftBits)) | 
								   (m_HRookMask   + (1 << m_HRookShiftBits  )));

	u32 m_value_;
};
