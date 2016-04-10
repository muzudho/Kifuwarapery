#pragma once

#include "../../header/n110_square__/n110_500_utilSquare.hpp"
#include "../../header/n113_piece___/n113_205_utilHandPiece.hpp"
#include "../../header/n220_position/n220_700_charToPieceUSI.hpp"

// xxxxxxxx xxxxxxxx xxxxxxxx x1111111  移動先
// xxxxxxxx xxxxxxxx xx111111 1xxxxxxx  移動元。駒打ちの際には、PieceType + SquareNum - 1
// xxxxxxxx xxxxxxxx x1xxxxxx xxxxxxxx  1 なら成り
// xxxxxxxx xxxx1111 xxxxxxxx xxxxxxxx  移動する駒の PieceType 駒打ちの際には使用しない。
// xxxxxxxx 1111xxxx xxxxxxxx xxxxxxxx  取られた駒の PieceType

// todo: piece to move と captured piece は指し手としてパックする必要あるの？
//       from, to , promo だけだったら、16bit で済む。
class Move {
public:

	Move();

	explicit Move(const u32 u) : value_(u) {}

	Move& operator = (const Move& m) { value_ = m.value_; return *this; }

	Move& operator = (const volatile Move& m) { value_ = m.value_; return *this; }

	// volatile Move& 型の *this を返すとなぜか警告が出るので、const Move& 型の m を返すことにする。
	const Move& operator = (const Move& m) volatile { value_ = m.value_; return m; }

	Move(const Move& m) { value_ = m.value_; }

	Move(const volatile Move& m) { value_ = m.value_; }

	// 移動先
	Square To() const;

	// 移動元
	Square From() const;

	// 移動元、移動先
	u32 FromAndTo() const;

	// 成り、移動元、移動先
	u32 ProFromAndTo() const;

	// 取った駒の種類
	PieceType GetCap() const;

	// 成るかどうか
	u32 IsPromotion() const;

	// 移動する駒の種類
	PieceType GetPieceTypeFrom() const;

	// 移動した後の駒の種類
	PieceType GetPieceTypeTo() const;

	// 移動前の PieceType を引数に取り、移動後の PieceType を返す。
	// 高速化の為、ptFrom が確定しているときに使用する。
	PieceType GetPieceTypeTo(const PieceType ptFrom) const;

	bool IsDrop() const;

	// 0xf00000 は 取られる駒のマスク
	bool IsCapture() const;

	// 0xf04000 は 取られる駒と成のマスク
	bool IsCaptureOrPromotion() const;

	bool IsCaptureOrPawnPromotion() const;

	// 打つ駒の種類
	PieceType GetPieceTypeDropped() const;

	PieceType GetPieceTypeFromOrDropped() const;

	HandPiece GetHandPieceDropped() const;

	// 値が入っているか。
	bool IsNone() const;

	// メンバ変数 value_ の取得
	u32 GetValue() const;

	Move operator |= (const Move rhs);

	Move operator | (const Move rhs) const;

	bool operator == (const Move rhs) const;
	bool operator != (const Move rhs) const;
	bool operator < (const Move rhs) const;
	std::string GetPromoteFlagToStringUSI() const;
	std::string ToUSI() const;
	std::string ToCSA() const;

	static Move GetMoveNone();
	static Move GetMoveNull();

	// 学習時に、正解の手のPV、その他の手のPVを MoveNone で区切りながら 1 次元配列に格納していく。
	// 格納するその他のPVの最後に MovePVsEnd を格納する。それをフラグに次の指し手に遷移する。
	// 正解のPV, MoveNone, その他0のPV, MoveNone, その他1のPV, MoveNone, MovePVsEnd という感じに並ぶ。
	static Move GetMovePVsEnd();

	static const u32 m_PromoteFlag = 1 << 14;
	static const u32 m_MoveNone = 0;
	static const u32 m_MoveNull = 129;
	static const u32 m_MovePVsEnd = 1 << 15; // for learn

private:
	u32 value_;
};
