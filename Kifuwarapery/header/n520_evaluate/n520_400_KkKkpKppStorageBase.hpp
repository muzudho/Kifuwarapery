#pragma once

#include "../n105_color___/n105_100_color.hpp"
#include "../n110_square__/n110_100_square.hpp"
#include "../n111_colorSq_/n111_500_ColorFileRank.h"
#include "../n112_pieceTyp/n112_050_pieceType.hpp"
#include "../n113_piece___/n113_155_convPiece.hpp"
#include "../n160_board___/n160_100_bitboard.hpp"
#include "../n160_board___/n160_230_setMaskBb.hpp"
#include "../n161_sqDistan/n161_500_squareDistance.hpp"
#include "../n220_position/n220_640_utilAttack.hpp"
#include "../n220_position/n220_750_charToPieceUSI.hpp"
#include "../n520_evaluate/n520_150_kppIndexArray.hpp"
#include "../n520_evaluate/n520_300_KPPBoardIndexStartToPiece.hpp"



template <typename Tl, typename Tr>
inline std::array<Tl, 2> operator += (std::array<Tl, 2>& lhs, const std::array<Tr, 2>& rhs) {
	lhs[0] += rhs[0];
	lhs[1] += rhs[1];
	return lhs;
}


template <typename Tl, typename Tr>
inline std::array<Tl, 2> operator -= (std::array<Tl, 2>& lhs, const std::array<Tr, 2>& rhs) {
	lhs[0] -= rhs[0];
	lhs[1] -= rhs[1];
	return lhs;
}


template <typename KPPType, typename KKPType, typename KKType>
struct KkKkpKppStorageBase {

	static const int m_R_Mid = 8; // 相対位置の中心のindex

	constexpr int MaxWeight() const { return 1 << 22; } // KPE自体が1/32の寄与。更にKPEの遠隔駒の利きが1マスごとに1/2に減衰する分(最大でKEEの際に8マス離れが2枚)
														// 更に重みを下げる場合、MaxWeightを更に大きくしておく必要がある。
														// なぜか clang で static const int MaxWeight を使っても Undefined symbols for architecture x86_64 と言われる。
	constexpr int TurnWeight() const { return 8; }

	//────────────────────────────────────────────────────────────────────────────────
	// KPP
	//────────────────────────────────────────────────────────────────────────────────
	// 冗長に配列を確保しているが、対称な関係にある時は常に若いindexの方にアクセスすることにする。
	// 例えば kpp だったら、k が優先的に小さくなるようする。左右の対称も含めてアクセス位置を決める。
	// ただし、kkp に関する項目 (kkp, r_kkp_b, r_kkp_h) のみ、GetP は味方の駒として扱うので、k0 < k1 となるとは限らない。
	struct KPPElements {
		// （＾ｑ＾）ファイル名に連動しているので、頭に m_ を付けてはいけない☆！
		KPPType dummy; // 一次元配列に変換したとき、符号で += を表すようにしているが、index = 0 の時は符号を付けられないので、ダミーを置く。

		KPPType kpp[SquareNoLeftNum][fe_end][fe_end];
		// 相対位置は[file][rank]の順
		KPPType r_kpp_bb[N31_PieceNone][17][17][N31_PieceNone][17][17];
		KPPType r_kpp_hb[fe_hand_end][N31_PieceNone][17][17];
		KPPType xpp[FileNoLeftNum][fe_end][fe_end];
		KPPType ypp[RankNum][fe_end][fe_end];
		KPPType pp[fe_end][fe_end];
		KPPType r_pp_bb[N31_PieceNone][N31_PieceNone][17][17];
		KPPType r_pp_hb[fe_hand_end][N31_PieceNone];

		// e は Effect の頭文字で利きを表す。(Control = 利き という説もあり。)
		// todo: 玉の利きは全く無視しているけれど、それで良いのか？
		KPPType kpe[SquareNoLeftNum][fe_end][g_COLOR_NUM][SquareNum];
		KPPType kee[SquareNoLeftNum][g_COLOR_NUM][SquareNum][g_COLOR_NUM][SquareNum];
		KPPType r_kpe_b[N31_PieceNone][17][17][g_COLOR_NUM][17][17];
		KPPType r_kpe_h[fe_hand_end][g_COLOR_NUM][17][17];
		KPPType r_kee[g_COLOR_NUM][17][17][g_COLOR_NUM][17][17];
		KPPType xpe[FileNoLeftNum][fe_end][g_COLOR_NUM][SquareNum];
		KPPType xee[FileNoLeftNum][g_COLOR_NUM][SquareNum][g_COLOR_NUM][SquareNum];
		KPPType ype[RankNum][fe_end][g_COLOR_NUM][SquareNum];
		KPPType yee[RankNum][g_COLOR_NUM][SquareNum][g_COLOR_NUM][SquareNum];
		KPPType pe[fe_end][g_COLOR_NUM][SquareNum];
		KPPType ee[g_COLOR_NUM][SquareNum][g_COLOR_NUM][SquareNum];
		KPPType r_pe_b[N31_PieceNone][g_COLOR_NUM][17][17];
		KPPType r_pe_h[fe_hand_end][g_COLOR_NUM];
		KPPType r_ee[g_COLOR_NUM][g_COLOR_NUM][17][17];
	};
	// （＾ｑ＾）ファイル名に連動しているので、頭に m_ を付けてはいけない☆！
	KPPElements kpps;

	//────────────────────────────────────────────────────────────────────────────────
	// KKP
	//────────────────────────────────────────────────────────────────────────────────
	struct KKPElements {
		// （＾ｑ＾）ファイル名に連動しているので、頭に m_ を付けてはいけない☆！
		KKPType dummy; // 一次元配列に変換したとき、符号で += を表すようにしているが、index = 0 の時は符号を付けられないので、ダミーを置く。
		KKPType kkp[SquareNoLeftNum][SquareNum][fe_end];
		KKPType kp[SquareNoLeftNum][fe_end];
		KKPType r_kkp_b[17][17][N31_PieceNone][17][17];
		KKPType r_kkp_h[17][17][fe_hand_end];
		KKPType r_kp_b[N31_PieceNone][17][17];
		KKPType r_kp_h[fe_hand_end];

		KKPType kke[SquareNoLeftNum][SquareNum][g_COLOR_NUM][SquareNum];
		KKPType ke[SquareNoLeftNum][g_COLOR_NUM][SquareNum];
		KKPType r_kke[17][17][g_COLOR_NUM][17][17];
		KKPType r_ke[g_COLOR_NUM][17][17];
	};
	// （＾ｑ＾）ファイル名に連動しているので、頭に m_ を付けてはいけない☆！
	KKPElements kkps;

	//────────────────────────────────────────────────────────────────────────────────
	// KKE
	//────────────────────────────────────────────────────────────────────────────────
	struct KKElements {
		// （＾ｑ＾）ファイル名に連動しているので、頭に m_ を付けてはいけない☆！
		KKType dummy; // 一次元配列に変換したとき、符号で += を表すようにしているが、index = 0 の時は符号を付けられないので、ダミーを置く。
		KKType kk[SquareNoLeftNum][SquareNum];
		KKType k[SquareNoLeftNum];
		KKType r_kk[17][17];
	};
	// （＾ｑ＾）ファイル名に連動しているので、頭に m_ を付けてはいけない☆！
	KKElements kks;

	// これらは↑のメンバ変数に一次元配列としてアクセスする為のもの。
	// 配列の要素数は上のstructのサイズから分かるはずだが無名structなのでsizeof()使いにくいから使わない。
	// 先頭さえ分かれば良いので要素数1で良い。
	KPPType*	GetKppOneArrayFirst(const u64 i) { return reinterpret_cast<KPPType*>(&kpps) + i; }
	KKPType*	GetKkpOneArrayFirst(const u64 i) { return reinterpret_cast<KKPType*>(&kkps) + i; }
	KKType*		GetKkOneArrayFirst(const u64 i) { return reinterpret_cast<KKType*>(&kks) + i; }

	// todo: これらややこしいし汚いので使わないようにする。
	//       型によっては kkps_begin_index などの値が異なる。
	//       ただ、end - begin のサイズは型によらず一定。
	constexpr size_t GetKpps_end_index() const { return sizeof(kpps) / sizeof(KPPType); }
	constexpr size_t GetKkps_end_index() const { return sizeof(kkps) / sizeof(KKPType); }
	constexpr size_t GetKks_end_index() const { return sizeof(kks) / sizeof(KKType); }

	static const int g_KPPIndicesMax = 3000;
	static const int g_KKPIndicesMax = 130;
	static const int g_KKIndicesMax = 7;

	// KPP に関する相対位置などの次元を落とした位置などのインデックスを全て返す。
	// 負のインデックスは、正のインデックスに変換した位置の点数を引く事を意味する。
	// 0 の時だけは正負が不明だが、0 は歩の持ち駒 0 枚を意味していて無効な値なので問題なし。
	// ptrdiff_t はインデックス、int は寄与の大きさ。MaxWeight分のいくつかで表記することにする。
	void CreateKppIndices(std::pair<ptrdiff_t, int> ret[g_KPPIndicesMax], Square ksq, int i, int j) {
		int retIdx = 0;
		// i == j のKP要素はKKPの方で行うので、こちらでは何も有効なindexを返さない。
		if (i == j) {
			ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
			assert(retIdx <= g_KPPIndicesMax);
			return;
		}
		if (j < i) std::swap(i, j);

		if (E1 < ksq) {
			ksq = ConvSquare::INVERSE_FILE40(ksq);
			i = UtilKppIndex::InverseFileIndexIfOnBoard(i);
			j = UtilKppIndex::InverseFileIndexIfOnBoard(j);
			if (j < i) std::swap(i, j);
		}
		else if (ConvSquare::TO_FILE10(ksq) == FileE) {
			assert(i < j);
			if (f_pawn <= i) {
				const int ibegin = UtilKppIndex::GetBegin(i);
				const Square isq = static_cast<Square>(i - ibegin);
				if (E1 < isq) {
					i = ibegin + ConvSquare::INVERSE_FILE40(isq);
					j = UtilKppIndex::InverseFileIndexOnBoard(j);
				}
				else if (ConvSquare::TO_FILE10(isq) == FileE) {
					j = UtilKppIndex::InverseFileIndexIfLefterThanMiddle(j);
				}
			}
		}
		if (j < i) std::swap(i, j);

#if defined EVAL_PHASE4
		ret[retIdx++] = std::make_pair(&kpps.kpp[ksq][i][j] - GetKppOneArrayFirst(0), MaxWeight());
		ret[retIdx++] = std::make_pair(&kpps.xpp[ConvSquare::TO_FILE10(ksq)][i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif

		assert(i < j);
		if (j < fe_hand_end) {
			// i, j 共に持ち駒
			// 相対位置無し。
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(&kpps.pp[i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE4
			ret[retIdx++] = std::make_pair(&kpps.ypp[ConvSquare::TO_RANK10(ksq)][i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif
		}
		else if (i < fe_hand_end) {
			// i 持ち駒、 j 盤上
			const int jbegin = UtilKppIndex::GetBegin(j);
			const Piece jpiece = g_kppBoardIndexStartToPiece.value(jbegin);
			const Square jsq = static_cast<Square>(j - jbegin);
			const Rank krank = ConvSquare::TO_RANK10(ksq);
			const File kfile = ConvSquare::TO_FILE10(ksq);
			const Rank jrank = ConvSquare::TO_RANK10(jsq);
			const File jfile = ConvSquare::TO_FILE10(jsq);
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(&kpps.r_kpp_hb[i][jpiece][m_R_Mid + -abs(kfile - jfile)][m_R_Mid + krank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE2
			ret[retIdx++] = std::make_pair(&kpps.r_pp_hb[i][jpiece] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(&kpps.pp[i][UtilKppIndex::InverseFileIndexIfLefterThanMiddle(j)] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE4
			ret[retIdx++] = std::make_pair(&kpps.ypp[krank][i][UtilKppIndex::InverseFileIndexIfLefterThanMiddle(j)] - GetKppOneArrayFirst(0), MaxWeight());
#endif

#if defined EVAL_PHASE1 || defined EVAL_PHASE3
			const Color jcolor = ConvPiece::TO_COLOR10(jpiece);
			const PieceType jpt = ConvPiece::TO_PIECE_TYPE10(jpiece);
			Bitboard jtoBB = g_setMaskBb.GetSetMaskBb(ksq).NotThisAnd(
				UtilAttack::GetAttacksFrom(jpt, jcolor, jsq, g_setMaskBb.GetSetMaskBb(ksq)));
			while (jtoBB.Exists1Bit()) {
				Square jto = jtoBB.PopFirstOneFromI9();
				if (kfile == FileE && E1 < jto)
					jto = ConvSquare::INVERSE_FILE40(jto);
				const int distance = g_squareDistance.GetSquareDistance(jsq, jto);
				// distance == 1 で 1/8 で 3bit シフトにする程度の寄与にする。
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(&kpps.kpe[ksq][i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
				ret[retIdx++] = std::make_pair(&kpps.xpe[kfile][i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
				const Rank jtorank = ConvSquare::TO_RANK10(jto);
				const File jtofile = ConvSquare::TO_FILE10(jto);
#if defined EVAL_PHASE1
				ret[retIdx++] = std::make_pair(&kpps.r_kpe_h[i][jcolor][m_R_Mid + -abs(kfile - jtofile)][m_R_Mid + krank - jtorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
				ret[retIdx++] = std::make_pair(&kpps.r_pe_h[i][jcolor] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
				ret[retIdx++] = std::make_pair(&kpps.pe[i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
				if (E1 < jto)
					jto = ConvSquare::INVERSE_FILE40(jto);
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(&kpps.ype[krank][i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
			}
#endif
		}
		else {
			// i, j 共に盤上
			const int ibegin = UtilKppIndex::GetBegin(i);
			const int jbegin = UtilKppIndex::GetBegin(j);
			const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
			const Piece jpiece = g_kppBoardIndexStartToPiece.value(jbegin);
			const Square isq = static_cast<Square>(i - ibegin);
			const Square jsq = static_cast<Square>(j - jbegin);
			const Rank krank = ConvSquare::TO_RANK10(ksq);
			const File kfile = ConvSquare::TO_FILE10(ksq);
			const Rank irank = ConvSquare::TO_RANK10(isq);
			const File ifile = ConvSquare::TO_FILE10(isq);
			const Rank jrank = ConvSquare::TO_RANK10(jsq);
			const File jfile = ConvSquare::TO_FILE10(jsq);
			File diff_file_ki = kfile - ifile;
			bool kfile_ifile_is_inversed = false;
			if (0 < diff_file_ki) {
				diff_file_ki = -diff_file_ki;
				kfile_ifile_is_inversed = true;
			}
			const File diff_file_kj =
				static_cast<File>(diff_file_ki == static_cast<File>(0) ? -abs(kfile - jfile) :
					kfile_ifile_is_inversed ? jfile - kfile : kfile - jfile);
			if (ipiece == jpiece) {
#if defined EVAL_PHASE3
				if (diff_file_kj < diff_file_ki || (diff_file_kj == diff_file_ki && -jrank < -irank))
					ret[retIdx++] = std::make_pair(&kpps.r_kpp_bb[jpiece][m_R_Mid + diff_file_kj][m_R_Mid + krank - jrank][ipiece][m_R_Mid + diff_file_ki][m_R_Mid + krank - irank] - GetKppOneArrayFirst(0), MaxWeight());
				else
					ret[retIdx++] = std::make_pair(&kpps.r_kpp_bb[ipiece][m_R_Mid + diff_file_ki][m_R_Mid + krank - irank][jpiece][m_R_Mid + diff_file_kj][m_R_Mid + krank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
				// 同じ駒の種類の時は、2駒の相対関係は上下がどちらになっても同じ点数であるべき。
#if defined EVAL_PHASE2
				ret[retIdx++] = std::make_pair(&kpps.r_pp_bb[ipiece][jpiece][m_R_Mid + -abs(ifile - jfile)][m_R_Mid + -abs(irank - jrank)] - GetKppOneArrayFirst(0), MaxWeight());
#endif
			}
			else {
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(&kpps.r_kpp_bb[ipiece][m_R_Mid + diff_file_ki][m_R_Mid + krank - irank][jpiece][m_R_Mid + diff_file_kj][m_R_Mid + krank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE2
				ret[retIdx++] = std::make_pair(&kpps.r_pp_bb[ipiece][jpiece][m_R_Mid + -abs(ifile - jfile)][m_R_Mid + irank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
			}

			auto func = [this, &retIdx, &ret](Square ksq, int ij, int ji) {
				const Rank krank = ConvSquare::TO_RANK10(ksq);
				const File kfile = ConvSquare::TO_FILE10(ksq);
				const int ijbegin = UtilKppIndex::GetBegin(ij);
				const int jibegin = UtilKppIndex::GetBegin(ji);
				const Piece ijpiece = g_kppBoardIndexStartToPiece.value(ijbegin);
				const Piece jipiece = g_kppBoardIndexStartToPiece.value(jibegin);
				const Square ijsq = static_cast<Square>(ij - ijbegin);
				const Square jisq = static_cast<Square>(ji - jibegin);

				const Color jicolor = ConvPiece::TO_COLOR10(jipiece);
				const PieceType jipt = ConvPiece::TO_PIECE_TYPE10(jipiece);
				const Bitboard mask = g_setMaskBb.GetSetMaskBb(ksq) | g_setMaskBb.GetSetMaskBb(ijsq);
				Bitboard jitoBB = mask.NotThisAnd(UtilAttack::GetAttacksFrom(jipt, jicolor, jisq, mask));
				while (jitoBB.Exists1Bit()) {
					Square jito = jitoBB.PopFirstOneFromI9();
					Square ijsq_tmp = ijsq;
					assert(ksq <= E1);
					if (ConvSquare::TO_FILE10(ksq) == FileE) {
						if (E1 < ijsq_tmp) {
							ij = UtilKppIndex::InverseFileIndexOnBoard(ij);
							ijsq_tmp = ConvSquare::INVERSE_FILE40(ijsq_tmp);
							jito = ConvSquare::INVERSE_FILE40(jito);
						}
						else if (ConvSquare::TO_FILE10(ijsq_tmp) == FileE)
							jito = ConvSquare::INVERSE_FILE40(jito);
					}
					const Rank ijrank = ConvSquare::TO_RANK10(ijsq_tmp);
					const File ijfile = ConvSquare::TO_FILE10(ijsq_tmp);
					const int distance = g_squareDistance.GetSquareDistance(jisq, jito);
#if defined EVAL_PHASE3
					ret[retIdx++] = std::make_pair(&kpps.kpe[ksq][ij][jicolor][jito] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
					ret[retIdx++] = std::make_pair(&kpps.xpe[ConvSquare::TO_FILE10(ksq)][ij][jicolor][jito] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
					const Rank jitorank = ConvSquare::TO_RANK10(jito);
					const File jitofile = ConvSquare::TO_FILE10(jito);
					{
						int ij_tmp = ij;
						int jito_tmp = jito;
						if (FileE < ijfile) {
							ij_tmp = UtilKppIndex::InverseFileIndexOnBoard(ij_tmp);
							jito_tmp = ConvSquare::INVERSE_FILE40(jito);
						}
						else if (FileE == ijfile && FileE < jitofile)
							jito_tmp = ConvSquare::INVERSE_FILE40(jito);

#if defined EVAL_PHASE3
						ret[retIdx++] = std::make_pair(
							&kpps.ype[ConvSquare::TO_RANK10(ksq)][ij_tmp][jicolor][jito_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4)
						);
#endif
					}

					File diff_file_kij = kfile - ijfile;
					bool kfile_ijfile_is_inversed = false;
					if (0 < diff_file_kij) {
						diff_file_kij = -diff_file_kij;
						kfile_ijfile_is_inversed = true;
					}
					const File diff_file_kjito =
						static_cast<File>(diff_file_kij == static_cast<File>(0) ? -abs(kfile - jitofile) :
							kfile_ijfile_is_inversed ? jitofile - kfile : kfile - jitofile);
#if defined EVAL_PHASE1
					ret[retIdx++] = std::make_pair(&kpps.r_kpe_b[ijpiece][m_R_Mid + diff_file_kij][m_R_Mid + krank - ijrank][jicolor][m_R_Mid + diff_file_kjito][m_R_Mid + krank - jitorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
					ret[retIdx++] = std::make_pair(&kpps.r_pe_b[ijpiece][jicolor][m_R_Mid + -abs(ijfile - jitofile)][m_R_Mid + ijrank - jitorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif

					int ij_tmp = ij;
					if (FileE < ijfile) {
						ij_tmp = UtilKppIndex::InverseFileIndexOnBoard(ij_tmp);
						jito = ConvSquare::INVERSE_FILE40(jito);
					}
					else if (FileE == ijfile && E1 < jito) {
						jito = ConvSquare::INVERSE_FILE40(jito);
					}
#if defined EVAL_PHASE1
					ret[retIdx++] = std::make_pair(&kpps.pe[ij_tmp][jicolor][jito] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
				}
			};
#if defined EVAL_PHASE1 || defined EVAL_PHASE3
			func(ksq, i, j);
			func(ksq, j, i);
#endif
			auto ee_func = [this, &retIdx, &ret](Square ksq, int i, int j) {
				assert(ksq <= E1);
				const Rank krank = ConvSquare::TO_RANK10(ksq);
				const File kfile = ConvSquare::TO_FILE10(ksq);
				auto color = [](int ij) {
					const int ijbegin = UtilKppIndex::GetBegin(ij);
					const Piece ijpiece = g_kppBoardIndexStartToPiece.value(ijbegin);
					const Color ijcolor = ConvPiece::TO_COLOR10(ijpiece);
					return ijcolor;
				};
				if (color(j) < color(i))
					std::swap(i, j);
				const int ibegin = UtilKppIndex::GetBegin(i);
				const int jbegin = UtilKppIndex::GetBegin(j);
				const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
				const Piece jpiece = g_kppBoardIndexStartToPiece.value(jbegin);
				const Square isq = static_cast<Square>(i - ibegin);
				const Square jsq = static_cast<Square>(j - jbegin);

				const Color icolor = ConvPiece::TO_COLOR10(ipiece);
				const Color jcolor = ConvPiece::TO_COLOR10(jpiece);
				const PieceType ipt = ConvPiece::TO_PIECE_TYPE10(ipiece);
				const PieceType jpt = ConvPiece::TO_PIECE_TYPE10(jpiece);
				const Bitboard imask = g_setMaskBb.GetSetMaskBb(ksq) | g_setMaskBb.GetSetMaskBb(jsq);
				const Bitboard jmask = g_setMaskBb.GetSetMaskBb(ksq) | g_setMaskBb.GetSetMaskBb(isq);
				Bitboard itoBB = imask.NotThisAnd(UtilAttack::GetAttacksFrom(jpt, icolor, isq, imask));
				Bitboard jtoBB = jmask.NotThisAnd(UtilAttack::GetAttacksFrom(jpt, jcolor, jsq, jmask));
				while (itoBB.Exists1Bit()) {
					const Square ito = itoBB.PopFirstOneFromI9();
					const int itodistance = g_squareDistance.GetSquareDistance(isq, ito);
					Bitboard jtoBB_tmp = jtoBB;
					while (jtoBB_tmp.Exists1Bit()) {
						const Square jto = jtoBB_tmp.PopFirstOneFromI9();
						const int jtodistance = g_squareDistance.GetSquareDistance(jsq, jto);
						const int distance = itodistance + jtodistance - 1;
						{
							Square ito_tmp = ito;
							Square jto_tmp = jto;
							if (kfile == FileE) {
								if (icolor == jcolor) {
									if (std::min(ConvSquare::INVERSE_FILE40(ito_tmp), ConvSquare::INVERSE_FILE40(jto_tmp)) < std::min(ito_tmp, jto_tmp)) {
										ito_tmp = ConvSquare::INVERSE_FILE40(ito_tmp);
										jto_tmp = ConvSquare::INVERSE_FILE40(jto_tmp);
									}
									if (jto_tmp < ito_tmp)
										std::swap(ito_tmp, jto_tmp);
								}
								else {
									if (E1 < ito_tmp) {
										ito_tmp = ConvSquare::INVERSE_FILE40(ito_tmp);
										jto_tmp = ConvSquare::INVERSE_FILE40(jto_tmp);
									}
									else if (ConvSquare::TO_FILE10(ito_tmp) == FileE && E1 < jto_tmp)
										jto_tmp = ConvSquare::INVERSE_FILE40(jto_tmp);
								}
							}
							else if (icolor == jcolor && jto_tmp < ito_tmp)
								std::swap(ito_tmp, jto_tmp);
#if defined EVAL_PHASE1
							// ee は数が多くなる為、重みを更に小さくする。
							ret[retIdx++] = std::make_pair(&kpps.kee[ksq][icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
							ret[retIdx++] = std::make_pair(&kpps.xee[kfile][icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
							File diff_file_kito = kfile - ConvSquare::TO_FILE10(ito_tmp);
							bool kfile_itofile_is_inversed = false;
							if (0 < diff_file_kito) {
								diff_file_kito = -diff_file_kito;
								kfile_itofile_is_inversed = true;
							}
							File diff_file_kjto =
								static_cast<File>(diff_file_kito == static_cast<File>(0) ? -abs(kfile - ConvSquare::TO_FILE10(jto_tmp)) :
									kfile_itofile_is_inversed ? ConvSquare::TO_FILE10(jto_tmp) - kfile : kfile - ConvSquare::TO_FILE10(jto_tmp));
							Rank diff_rank_kito = krank - ConvSquare::TO_RANK10(ito_tmp);
							Rank diff_rank_kjto = krank - ConvSquare::TO_RANK10(jto_tmp);
							ColorFileRank iColorFileRank = ColorFileRank(icolor, diff_file_kito, diff_rank_kito);
							ColorFileRank jColorFileRank = ColorFileRank(jcolor, diff_file_kjto, diff_rank_kjto);
							if (jColorFileRank.GetOrder() < iColorFileRank.GetOrder())
								std::swap(iColorFileRank, jColorFileRank);
#if defined EVAL_PHASE1
							ret[retIdx++] = std::make_pair(&kpps.r_kee[iColorFileRank.m_color][m_R_Mid + iColorFileRank.m_file][m_R_Mid + iColorFileRank.m_rank][jColorFileRank.m_color][m_R_Mid + jColorFileRank.m_file][m_R_Mid + jColorFileRank.m_rank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
						}
						Square ito_tmp = ito;
						Square jto_tmp = jto;
						if (icolor == jcolor) {
							if (std::min(ConvSquare::INVERSE_FILE40(ito_tmp), ConvSquare::INVERSE_FILE40(jto_tmp)) < std::min(ito_tmp, jto_tmp)) {
								ito_tmp = ConvSquare::INVERSE_FILE40(ito_tmp);
								jto_tmp = ConvSquare::INVERSE_FILE40(jto_tmp);
							}
							if (jto_tmp < ito_tmp)
								std::swap(ito_tmp, jto_tmp);
						}
						else {
							if (E1 < ito_tmp) {
								ito_tmp = ConvSquare::INVERSE_FILE40(ito_tmp);
								jto_tmp = ConvSquare::INVERSE_FILE40(jto_tmp);
							}
							else if (ConvSquare::TO_FILE10(ito_tmp) == FileE && E1 < jto_tmp)
								jto_tmp = ConvSquare::INVERSE_FILE40(jto_tmp);
						}
#if defined EVAL_PHASE1
						ret[retIdx++] = std::make_pair(&kpps.ee[icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
						ret[retIdx++] = std::make_pair(&kpps.yee[krank][icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
						const File itofile = ConvSquare::TO_FILE10(ito_tmp);
						const Rank itorank = ConvSquare::TO_RANK10(ito_tmp);
						const File jtofile = ConvSquare::TO_FILE10(jto_tmp);
						const Rank jtorank = ConvSquare::TO_RANK10(jto_tmp);
#if defined EVAL_PHASE1
						ret[retIdx++] = std::make_pair(&kpps.r_ee[icolor][jcolor][m_R_Mid + abs(-itofile - jtofile)][m_R_Mid + itorank - jtorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
					}
				}
			};
#if defined EVAL_PHASE1
			ee_func(ksq, i, j);
#endif

			if (ifile == FileE) {
				// ppに関してiが5筋なのでjだけ左右反転しても構わない。
				j = UtilKppIndex::InverseFileIndexIfLefterThanMiddle(j);
				if (j < i) std::swap(i, j);
			}
			else if ((E1 < isq)
				|| (ibegin == jbegin && ConvSquare::INVERSE_FILE40(jsq) < isq))
			{
				// ppに関してiを左右反転するのでjも左右反転する。
				i = UtilKppIndex::InverseFileIndexOnBoard(i);
				j = UtilKppIndex::InverseFileIndexOnBoard(j);
				if (j < i) std::swap(i, j);
			}
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(&kpps.pp[i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE4
			ret[retIdx++] = std::make_pair(&kpps.ypp[krank][i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif
		}

		ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
		assert(retIdx <= g_KPPIndicesMax);
	}


	void CreateKkpIndices(std::pair<ptrdiff_t, int> ret[g_KKPIndicesMax], Square ksq0, Square ksq1, int i) {
		int retIdx = 0;
		if (ksq0 == ksq1) {
			ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
			assert(retIdx <= g_KKPIndicesMax);
			return;
		}
		auto kp_func = [this, &retIdx, &ret](Square ksq, int i, int sign) {
			if (E1 < ksq) {
				ksq = ConvSquare::INVERSE_FILE40(ksq);
				i = UtilKppIndex::InverseFileIndexIfOnBoard(i);
			}
			else if (ConvSquare::TO_FILE10(ksq) == FileE)
				i = UtilKppIndex::InverseFileIndexIfLefterThanMiddle(i);
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(sign*(&kkps.kp[ksq][i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif
			auto r_kp_func = [this, &retIdx, &ret](Square ksq, int i, int sign) {
				if (i < fe_hand_end) {
#if defined EVAL_PHASE2
					ret[retIdx++] = std::make_pair(sign*(&kkps.r_kp_h[i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif
				}
				else {
					const int ibegin = UtilKppIndex::GetBegin(i);
					const Square isq = static_cast<Square>(i - ibegin);
					const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
#if defined EVAL_PHASE2
					ret[retIdx++] = std::make_pair(sign*(&kkps.r_kp_b[ipiece][m_R_Mid + -abs(ConvSquare::TO_FILE10(ksq) - ConvSquare::TO_FILE10(isq))][m_R_Mid + ConvSquare::TO_RANK10(ksq) - ConvSquare::TO_RANK10(isq)] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif

#if defined EVAL_PHASE1
					const PieceType ipt = ConvPiece::TO_PIECE_TYPE10(ipiece);
					const Color icolor = ConvPiece::TO_COLOR10(ipiece);
					Bitboard itoBB = g_setMaskBb.GetSetMaskBb(ksq).NotThisAnd(
						UtilAttack::GetAttacksFrom(ipt, icolor, isq, g_setMaskBb.GetSetMaskBb(ksq)));
					while (itoBB.Exists1Bit()) {
						Square ito = itoBB.PopFirstOneFromI9();
						const int distance = g_squareDistance.GetSquareDistance(isq, ito);
						ret[retIdx++] = std::make_pair(sign*(&kkps.r_ke[icolor][m_R_Mid + -abs(ConvSquare::TO_FILE10(ksq) - ConvSquare::TO_FILE10(ito))][m_R_Mid + ConvSquare::TO_RANK10(ksq) - ConvSquare::TO_RANK10(ito)] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
					}
#endif
				}
			};
			r_kp_func(ksq, i, sign);
#if defined EVAL_PHASE1
			if (f_pawn <= i) {
				const int ibegin = UtilKppIndex::GetBegin(i);
				const Square isq = static_cast<Square>(i - ibegin);
				const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
				const PieceType ipt = ConvPiece::TO_PIECE_TYPE10(ipiece);
				const Color icolor = ConvPiece::TO_COLOR10(ipiece);

				Bitboard itoBB = g_setMaskBb.GetSetMaskBb(ksq).NotThisAnd(
					UtilAttack::GetAttacksFrom(ipt, icolor, isq, g_setMaskBb.GetSetMaskBb(ksq)));
				while (itoBB.Exists1Bit()) {
					Square ito = itoBB.PopFirstOneFromI9();
					const int distance = g_squareDistance.GetSquareDistance(isq, ito);
					if (ConvSquare::TO_FILE10(ksq) == FileE && E1 < ito)
						ito = ConvSquare::INVERSE_FILE40(ito);
					ret[retIdx++] = std::make_pair(sign*(&kkps.ke[ksq][icolor][ito] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
				}
			}
#endif
		};

		kp_func(ksq0, i, 1);
		{
			const int begin = UtilKppIndex::GetBegin(i);
			const int opp_begin = UtilKppIndex::ToOpponentBegin(i);
			const int tmp_i = (begin < fe_hand_end ? opp_begin + (i - begin) : opp_begin + ConvSquare::INVERSE10(static_cast<Square>(i - begin)));
			kp_func(ConvSquare::INVERSE10(ksq1), tmp_i, -1);
		}

		int sign = 1;
		if (!UtilKppIndex::IsBlack(i)) {
			const Square tmp = ksq0;
			ksq0 = ConvSquare::INVERSE10(ksq1);
			ksq1 = ConvSquare::INVERSE10(tmp);
			const int ibegin = UtilKppIndex::GetBegin(i);
			const int opp_ibegin = UtilKppIndex::KppWhiteIndexToBlackBegin(i);
			i = opp_ibegin + (i < fe_hand_end ? i - ibegin : ConvSquare::INVERSE10(static_cast<Square>(i - ibegin)));
			sign = -1;
		}
		if (E1 < ksq0) {
			ksq0 = ConvSquare::INVERSE_FILE40(ksq0);
			ksq1 = ConvSquare::INVERSE_FILE40(ksq1);
			i = UtilKppIndex::InverseFileIndexIfOnBoard(i);
		}
		else if (ConvSquare::TO_FILE10(ksq0) == FileE && E1 < ksq1) {
			ksq1 = ConvSquare::INVERSE_FILE40(ksq1);
			i = UtilKppIndex::InverseFileIndexIfOnBoard(i);
		}
		else if (ConvSquare::TO_FILE10(ksq0) == FileE && ConvSquare::TO_FILE10(ksq1) == FileE) {
			i = UtilKppIndex::InverseFileIndexIfLefterThanMiddle(i);
		}
#if defined EVAL_PHASE4
		ret[retIdx++] = std::make_pair(sign*(&kkps.kkp[ksq0][ksq1][i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif

#if defined EVAL_PHASE1 || defined EVAL_PHASE3
		const Rank diff_rank_k0k1 = ConvSquare::TO_RANK10(ksq0) - ConvSquare::TO_RANK10(ksq1);
		File diff_file_k0k1 = ConvSquare::TO_FILE10(ksq0) - ConvSquare::TO_FILE10(ksq1);
		if (i < fe_hand_end) {
			if (0 < diff_file_k0k1)
				diff_file_k0k1 = -diff_file_k0k1;
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(sign*(&kkps.r_kkp_h[m_R_Mid + diff_file_k0k1][m_R_Mid + diff_rank_k0k1][i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif
		}
		else {
			const int ibegin = UtilKppIndex::GetBegin(i);
			const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
			Square isq = static_cast<Square>(i - ibegin);
			const Rank diff_rank_k0i = ConvSquare::TO_RANK10(ksq0) - ConvSquare::TO_RANK10(isq);
			File diff_file_k0i = ConvSquare::TO_FILE10(ksq0) - ConvSquare::TO_FILE10(isq);

			const Color icolor = ConvPiece::TO_COLOR10(ipiece);
			const PieceType ipt = ConvPiece::TO_PIECE_TYPE10(ipiece);
			const Bitboard mask = g_setMaskBb.GetSetMaskBb(ksq0) | g_setMaskBb.GetSetMaskBb(ksq1);
			Bitboard itoBB = mask.NotThisAnd(UtilAttack::GetAttacksFrom(ipt, icolor, isq, mask));
			while (itoBB.Exists1Bit()) {
				Square ito = itoBB.PopFirstOneFromI9();
				const int distance = g_squareDistance.GetSquareDistance(isq, ito);
				if (ConvSquare::TO_FILE10(ksq0) == FileE && ConvSquare::TO_FILE10(ksq1) == FileE && E1 < ito)
					ito = ConvSquare::INVERSE_FILE40(ito);
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(sign*(&kkps.kke[ksq0][ksq1][icolor][ito] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
#endif
#if defined EVAL_PHASE1
				File diff_file_k0k1_tmp = diff_file_k0k1;
				File diff_file_k0ito = ConvSquare::TO_FILE10(ksq0) - ConvSquare::TO_FILE10(ito);
				Rank diff_rank_k0ito = ConvSquare::TO_RANK10(ksq0) - ConvSquare::TO_RANK10(ito);
				if (0 < diff_file_k0k1_tmp) {
					diff_file_k0k1_tmp = -diff_file_k0k1_tmp;
					diff_file_k0ito = -diff_file_k0ito;
				}
				else if (0 == diff_file_k0k1_tmp && 0 < diff_file_k0ito)
					diff_file_k0ito = -diff_file_k0ito;
				ret[retIdx++] = std::make_pair(sign*(&kkps.r_kke[m_R_Mid + diff_file_k0k1_tmp][m_R_Mid + diff_rank_k0k1][icolor][m_R_Mid + diff_file_k0ito][m_R_Mid + diff_rank_k0ito] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
#endif
			}

			if (0 < diff_file_k0k1) {
				diff_file_k0k1 = -diff_file_k0k1;
				diff_file_k0i = -diff_file_k0i;
			}
			else if (0 == diff_file_k0k1 && 0 < diff_file_k0i) {
				diff_file_k0i = -diff_file_k0i;
			}
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(sign*(&kkps.r_kkp_b[m_R_Mid + diff_file_k0k1][m_R_Mid + diff_rank_k0k1][ipiece][m_R_Mid + diff_file_k0i][m_R_Mid + diff_rank_k0i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif
		}
#endif
		ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
		assert(retIdx <= g_KKPIndicesMax);
	}


	void CreateKkIndices(std::pair<ptrdiff_t, int> ret[g_KKIndicesMax], Square ksq0, Square ksq1) {
		int retIdx = 0;
#if defined EVAL_PHASE1
		ret[retIdx++] = std::make_pair(&kks.k[std::min(ksq0, ConvSquare::INVERSE_FILE40(ksq0))] - GetKkOneArrayFirst(0), MaxWeight());
		ret[retIdx++] = std::make_pair(-(&kks.k[std::min(ConvSquare::INVERSE10(ksq1), ConvSquare::INVERSE_FILE40(ConvSquare::INVERSE10(ksq1)))] - GetKkOneArrayFirst(0)), MaxWeight());
#endif

		auto kk_func = [this, &retIdx, &ret](Square ksq0, Square ksq1, int sign) {
			{
				// 常に ksq0 < ksq1 となるテーブルにアクセスする為の変換
				const Square ksq0Arr[] = {
					ksq0,
					ConvSquare::INVERSE_FILE40(ksq0),
				};
				const Square ksq1Arr[] = {
					ConvSquare::INVERSE10(ksq1),
					ConvSquare::INVERSE10(ConvSquare::INVERSE_FILE40(ksq1)),
				};
				auto ksq0ArrIdx = std::min_element(std::begin(ksq0Arr), std::end(ksq0Arr)) - std::begin(ksq0Arr);
				auto ksq1ArrIdx = std::min_element(std::begin(ksq1Arr), std::end(ksq1Arr)) - std::begin(ksq1Arr);
				if (ksq0Arr[ksq0ArrIdx] <= ksq1Arr[ksq1ArrIdx]) {
					ksq0 = ksq0Arr[ksq0ArrIdx];
					ksq1 = ConvSquare::INVERSE10(ksq1Arr[ksq0ArrIdx]);
				}
				else {
					sign = -sign; // ksq0 と ksq1 を入れ替えるので符号反転
					ksq0 = ksq1Arr[ksq1ArrIdx];
					ksq1 = ConvSquare::INVERSE10(ksq0Arr[ksq1ArrIdx]);
				}
			}
			const File kfile0 = ConvSquare::TO_FILE10(ksq0);
			const Rank krank0 = ConvSquare::TO_RANK10(ksq0);
			const File kfile1 = ConvSquare::TO_FILE10(ksq1);
			const Rank krank1 = ConvSquare::TO_RANK10(ksq1);
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(sign*(&kks.kk[ksq0][ksq1] - GetKkOneArrayFirst(0)), MaxWeight());
#endif
#if defined EVAL_PHASE2
			ret[retIdx++] = std::make_pair(sign*(&kks.r_kk[m_R_Mid + kfile0 - kfile1][m_R_Mid + krank0 - krank1] - GetKkOneArrayFirst(0)), MaxWeight());
#endif
			assert(ksq0 <= E1);
			assert(kfile0 - kfile1 <= 0);
		};
		kk_func(ksq0, ksq1, 1);
		kk_func(ConvSquare::INVERSE10(ksq1), ConvSquare::INVERSE10(ksq0), -1);
		ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
		assert(retIdx <= g_KKIndicesMax);
	}


	void clear() { memset(this, 0, sizeof(*this)); } // float 型とかだと規格的に 0 は保証されなかった気がするが実用上問題ないだろう。


};

