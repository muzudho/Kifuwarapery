#pragma once


#include <array>	// std::array
#include "../../header/n220_position/n220_650_position.hpp"


struct RawEvaluater {
	std::array<float, 2> kpp_raw[SquareNum][fe_end][fe_end];
	std::array<float, 2> kkp_raw[SquareNum][SquareNum][fe_end];
	std::array<float, 2> kk_raw[SquareNum][SquareNum];

	//────────────────────────────────────────────────────────────────────────────────────────────────────
	// インク・パーラム
	//────────────────────────────────────────────────────────────────────────────────────────────────────
	void incParam(const Position& pos, const std::array<double, 2>& dinc) {
		const Square sq_bk = pos.GetKingSquare(Black);
		const Square sq_wk = pos.GetKingSquare(White);
		const int* GetList0 = pos.GetCplist0();
		const int* GetList1 = pos.GetCplist1();
		const std::array<float, 2> f = { { static_cast<float>(dinc[0] / g_FVScale), static_cast<float>(dinc[1] / g_FVScale) } };

		kk_raw[sq_bk][sq_wk] += f;
		for (int i = 0; i < pos.GetNlist(); ++i) {
			const int k0 = GetList0[i];
			const int k1 = GetList1[i];
			for (int j = 0; j < i; ++j) {
				const int l0 = GetList0[j];
				const int l1 = GetList1[j];
				kpp_raw[sq_bk][k0][l0] += f;
				kpp_raw[ConvSquare::INVERSE10(sq_wk)][k1][l1][0] -= f[0];
				kpp_raw[ConvSquare::INVERSE10(sq_wk)][k1][l1][1] += f[1];
			}
			kkp_raw[sq_bk][sq_wk][k0] += f;
		}
	}

	void Clear() { memset(this, 0, sizeof(*this)); } // float 型とかだと規格的に 0 は保証されなかった気がするが実用上問題ないだろう。
};

