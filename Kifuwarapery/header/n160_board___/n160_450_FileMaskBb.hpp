#pragma once


#include "../n160_board___/n160_100_bitboard.hpp"
//#include "../n160_board___/n160_600_bitboardAll.hpp"


class FileMaskBb {
public:

	const Bitboard m_I = Bitboard(UINT64_C(0x1ff) << (9 * 0), 0);
	const Bitboard m_H = Bitboard(UINT64_C(0x1ff) << (9 * 1), 0);
	const Bitboard m_G = Bitboard(UINT64_C(0x1ff) << (9 * 2), 0);
	const Bitboard m_F = Bitboard(UINT64_C(0x1ff) << (9 * 3), 0);
	const Bitboard m_E = Bitboard(UINT64_C(0x1ff) << (9 * 4), 0);
	const Bitboard m_D = Bitboard(UINT64_C(0x1ff) << (9 * 5), 0);
	const Bitboard m_C = Bitboard(UINT64_C(0x1ff) << (9 * 6), 0);
	const Bitboard m_B = Bitboard(0, 0x1ff << (9 * 0));
	const Bitboard m_A = Bitboard(0, 0x1ff << (9 * 1));

	const Bitboard m_fileMask[FileNum] = {
		this->m_I,
		this->m_H,
		this->m_G,
		this->m_F,
		this->m_E,
		this->m_D,
		this->m_C,
		this->m_B,
		this->m_A
	};

public:
	inline Bitboard GetFileMask(const File f) const {
		return this->m_fileMask[f];
	}

	template <File F>
	inline Bitboard GetFileMask() const {
		static_assert(FileI <= F && F <= FileA, "");
		return (F == FileI ? this->m_I
			: F == FileH ? this->m_H
			: F == FileG ? this->m_G
			: F == FileF ? this->m_F
			: F == FileE ? this->m_E
			: F == FileD ? this->m_D
			: F == FileC ? this->m_C
			: F == FileB ? this->m_B
			: /*F == FileA ?*/ this->m_A);
	}

	// 直接テーブル引きすべきだと思う。
	inline Bitboard GetSquareFileMask(const Square sq) const {
		const File f = UtilSquare::ToFile(sq);
		return this->GetFileMask(f);
	}

};


// クラス定義のあとに書くとビルドできるぜ☆（＾ｑ＾）
extern const FileMaskBb g_fileMaskBb;
