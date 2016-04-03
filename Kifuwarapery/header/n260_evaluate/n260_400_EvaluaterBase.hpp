#pragma once

#include "../../header/n111_colorSq_/n111_500_ColorFileRank.h"
#include "../../header/n220_position/n220_500_charToPieceUSI.hpp"
#include "../../header/n260_evaluate/n260_200_evaluate01.hpp"
#include "../../header/n260_evaluate/n260_300_KPPBoardIndexStartToPiece.hpp"


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
struct EvaluaterBase {
	static const int R_Mid = 8; // 相対位置の中心のindex
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
		KPPType dummy; // 一次元配列に変換したとき、符号で += を表すようにしているが、index = 0 の時は符号を付けられないので、ダミーを置く。
		KPPType kpp[SquareNoLeftNum][fe_end][fe_end];
		// 相対位置は[file][rank]の順
		KPPType r_kpp_bb[PieceNone][17][17][PieceNone][17][17];
		KPPType r_kpp_hb[fe_hand_end][PieceNone][17][17];
		KPPType xpp[FileNoLeftNum][fe_end][fe_end];
		KPPType ypp[RankNum][fe_end][fe_end];
		KPPType pp[fe_end][fe_end];
		KPPType r_pp_bb[PieceNone][PieceNone][17][17];
		KPPType r_pp_hb[fe_hand_end][PieceNone];

		// e は Effect の頭文字で利きを表す。(Control = 利き という説もあり。)
		// todo: 玉の利きは全く無視しているけれど、それで良いのか？
		KPPType kpe[SquareNoLeftNum][fe_end][ColorNum][SquareNum];
		KPPType kee[SquareNoLeftNum][ColorNum][SquareNum][ColorNum][SquareNum];
		KPPType r_kpe_b[PieceNone][17][17][ColorNum][17][17];
		KPPType r_kpe_h[fe_hand_end][ColorNum][17][17];
		KPPType r_kee[ColorNum][17][17][ColorNum][17][17];
		KPPType xpe[FileNoLeftNum][fe_end][ColorNum][SquareNum];
		KPPType xee[FileNoLeftNum][ColorNum][SquareNum][ColorNum][SquareNum];
		KPPType ype[RankNum][fe_end][ColorNum][SquareNum];
		KPPType yee[RankNum][ColorNum][SquareNum][ColorNum][SquareNum];
		KPPType pe[fe_end][ColorNum][SquareNum];
		KPPType ee[ColorNum][SquareNum][ColorNum][SquareNum];
		KPPType r_pe_b[PieceNone][ColorNum][17][17];
		KPPType r_pe_h[fe_hand_end][ColorNum];
		KPPType r_ee[ColorNum][ColorNum][17][17];
	};
	KPPElements kpps;

	//────────────────────────────────────────────────────────────────────────────────
	// KKP
	//────────────────────────────────────────────────────────────────────────────────
	struct KKPElements {
		KKPType dummy; // 一次元配列に変換したとき、符号で += を表すようにしているが、index = 0 の時は符号を付けられないので、ダミーを置く。
		KKPType kkp[SquareNoLeftNum][SquareNum][fe_end];
		KKPType kp[SquareNoLeftNum][fe_end];
		KKPType r_kkp_b[17][17][PieceNone][17][17];
		KKPType r_kkp_h[17][17][fe_hand_end];
		KKPType r_kp_b[PieceNone][17][17];
		KKPType r_kp_h[fe_hand_end];

		KKPType kke[SquareNoLeftNum][SquareNum][ColorNum][SquareNum];
		KKPType ke[SquareNoLeftNum][ColorNum][SquareNum];
		KKPType r_kke[17][17][ColorNum][17][17];
		KKPType r_ke[ColorNum][17][17];
	};
	KKPElements kkps;

	//────────────────────────────────────────────────────────────────────────────────
	// KKE
	//────────────────────────────────────────────────────────────────────────────────
	struct KKElements {
		KKType dummy; // 一次元配列に変換したとき、符号で += を表すようにしているが、index = 0 の時は符号を付けられないので、ダミーを置く。
		KKType kk[SquareNoLeftNum][SquareNum];
		KKType k[SquareNoLeftNum];
		KKType r_kk[17][17];
	};
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
	constexpr size_t kpps_end_index() const { return sizeof(kpps) / sizeof(KPPType); }
	constexpr size_t kkps_end_index() const { return sizeof(kkps) / sizeof(KKPType); }
	constexpr size_t kks_end_index() const { return sizeof(kks) / sizeof(KKType); }

	static const int KPPIndicesMax = 3000;
	static const int KKPIndicesMax = 130;
	static const int KKIndicesMax = 7;

	// KPP に関する相対位置などの次元を落とした位置などのインデックスを全て返す。
	// 負のインデックスは、正のインデックスに変換した位置の点数を引く事を意味する。
	// 0 の時だけは正負が不明だが、0 は歩の持ち駒 0 枚を意味していて無効な値なので問題なし。
	// ptrdiff_t はインデックス、int は寄与の大きさ。MaxWeight分のいくつかで表記することにする。
	void CreateKppIndices(std::pair<ptrdiff_t, int> ret[KPPIndicesMax], Square ksq, int i, int j) {
		int retIdx = 0;
		// i == j のKP要素はKKPの方で行うので、こちらでは何も有効なindexを返さない。
		if (i == j) {
			ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
			assert(retIdx <= KPPIndicesMax);
			return;
		}
		if (j < i) std::swap(i, j);

		if (E1 < ksq) {
			ksq = UtilSquare::InverseFile(ksq);
			i = Evaluation01::inverseFileIndexIfOnBoard(i);
			j = Evaluation01::inverseFileIndexIfOnBoard(j);
			if (j < i) std::swap(i, j);
		}
		else if (UtilSquare::ToFile(ksq) == FileE) {
			assert(i < j);
			if (f_pawn <= i) {
				const int ibegin = Evaluation01::kppIndexBegin(i);
				const Square isq = static_cast<Square>(i - ibegin);
				if (E1 < isq) {
					i = ibegin + UtilSquare::InverseFile(isq);
					j = Evaluation01::inverseFileIndexOnBoard(j);
				}
				else if (UtilSquare::ToFile(isq) == FileE) {
					j = Evaluation01::inverseFileIndexIfLefterThanMiddle(j);
				}
			}
		}
		if (j < i) std::swap(i, j);

#if defined EVAL_PHASE4
		ret[retIdx++] = std::make_pair(&kpps.kpp[ksq][i][j] - GetKppOneArrayFirst(0), MaxWeight());
		ret[retIdx++] = std::make_pair(&kpps.xpp[UtilSquare::ToFile(ksq)][i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif

		assert(i < j);
		if (j < fe_hand_end) {
			// i, j 共に持ち駒
			// 相対位置無し。
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(&kpps.pp[i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE4
			ret[retIdx++] = std::make_pair(&kpps.ypp[UtilSquare::ToRank(ksq)][i][j] - GetKppOneArrayFirst(0), MaxWeight());
#endif
		}
		else if (i < fe_hand_end) {
			// i 持ち駒、 j 盤上
			const int jbegin = Evaluation01::kppIndexBegin(j);
			const Piece jpiece = g_kppBoardIndexStartToPiece.value(jbegin);
			const Square jsq = static_cast<Square>(j - jbegin);
			const Rank krank = UtilSquare::ToRank(ksq);
			const File kfile = UtilSquare::ToFile(ksq);
			const Rank jrank = UtilSquare::ToRank(jsq);
			const File jfile = UtilSquare::ToFile(jsq);
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(&kpps.r_kpp_hb[i][jpiece][R_Mid + -abs(kfile - jfile)][R_Mid + krank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE2
			ret[retIdx++] = std::make_pair(&kpps.r_pp_hb[i][jpiece] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(&kpps.pp[i][Evaluation01::inverseFileIndexIfLefterThanMiddle(j)] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE4
			ret[retIdx++] = std::make_pair(&kpps.ypp[krank][i][Evaluation01::inverseFileIndexIfLefterThanMiddle(j)] - GetKppOneArrayFirst(0), MaxWeight());
#endif

#if defined EVAL_PHASE1 || defined EVAL_PHASE3
			const Color jcolor = UtilPiece::ToColor(jpiece);
			const PieceType jpt = UtilPiece::ToPieceType(jpiece);
			Bitboard jtoBB = Bitboard::setMaskBB(ksq).notThisAnd(
				Position::attacksFrom(jpt, jcolor, jsq, Bitboard::setMaskBB(ksq)));
			while (jtoBB.isNot0()) {
				Square jto = jtoBB.firstOneFromI9();
				if (kfile == FileE && E1 < jto)
					jto = UtilSquare::InverseFile(jto);
				const int distance = UtilSquare::GetSquareDistance(jsq, jto);
				// distance == 1 で 1/8 で 3bit シフトにする程度の寄与にする。
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(&kpps.kpe[ksq][i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
				ret[retIdx++] = std::make_pair(&kpps.xpe[kfile][i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
				const Rank jtorank = UtilSquare::ToRank(jto);
				const File jtofile = UtilSquare::ToFile(jto);
#if defined EVAL_PHASE1
				ret[retIdx++] = std::make_pair(&kpps.r_kpe_h[i][jcolor][R_Mid + -abs(kfile - jtofile)][R_Mid + krank - jtorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
				ret[retIdx++] = std::make_pair(&kpps.r_pe_h[i][jcolor] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
				ret[retIdx++] = std::make_pair(&kpps.pe[i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
				if (E1 < jto)
					jto = UtilSquare::InverseFile(jto);
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(&kpps.ype[krank][i][jcolor][jto] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
			}
#endif
		}
		else {
			// i, j 共に盤上
			const int ibegin = Evaluation01::kppIndexBegin(i);
			const int jbegin = Evaluation01::kppIndexBegin(j);
			const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
			const Piece jpiece = g_kppBoardIndexStartToPiece.value(jbegin);
			const Square isq = static_cast<Square>(i - ibegin);
			const Square jsq = static_cast<Square>(j - jbegin);
			const Rank krank = UtilSquare::ToRank(ksq);
			const File kfile = UtilSquare::ToFile(ksq);
			const Rank irank = UtilSquare::ToRank(isq);
			const File ifile = UtilSquare::ToFile(isq);
			const Rank jrank = UtilSquare::ToRank(jsq);
			const File jfile = UtilSquare::ToFile(jsq);
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
					ret[retIdx++] = std::make_pair(&kpps.r_kpp_bb[jpiece][R_Mid + diff_file_kj][R_Mid + krank - jrank][ipiece][R_Mid + diff_file_ki][R_Mid + krank - irank] - GetKppOneArrayFirst(0), MaxWeight());
				else
					ret[retIdx++] = std::make_pair(&kpps.r_kpp_bb[ipiece][R_Mid + diff_file_ki][R_Mid + krank - irank][jpiece][R_Mid + diff_file_kj][R_Mid + krank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
				// 同じ駒の種類の時は、2駒の相対関係は上下がどちらになっても同じ点数であるべき。
#if defined EVAL_PHASE2
				ret[retIdx++] = std::make_pair(&kpps.r_pp_bb[ipiece][jpiece][R_Mid + -abs(ifile - jfile)][R_Mid + -abs(irank - jrank)] - GetKppOneArrayFirst(0), MaxWeight());
#endif
			}
			else {
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(&kpps.r_kpp_bb[ipiece][R_Mid + diff_file_ki][R_Mid + krank - irank][jpiece][R_Mid + diff_file_kj][R_Mid + krank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
#if defined EVAL_PHASE2
				ret[retIdx++] = std::make_pair(&kpps.r_pp_bb[ipiece][jpiece][R_Mid + -abs(ifile - jfile)][R_Mid + irank - jrank] - GetKppOneArrayFirst(0), MaxWeight());
#endif
			}

			auto func = [this, &retIdx, &ret](Square ksq, int ij, int ji) {
				const Rank krank = UtilSquare::ToRank(ksq);
				const File kfile = UtilSquare::ToFile(ksq);
				const int ijbegin = Evaluation01::kppIndexBegin(ij);
				const int jibegin = Evaluation01::kppIndexBegin(ji);
				const Piece ijpiece = g_kppBoardIndexStartToPiece.value(ijbegin);
				const Piece jipiece = g_kppBoardIndexStartToPiece.value(jibegin);
				const Square ijsq = static_cast<Square>(ij - ijbegin);
				const Square jisq = static_cast<Square>(ji - jibegin);

				const Color jicolor = UtilPiece::ToColor(jipiece);
				const PieceType jipt = UtilPiece::ToPieceType(jipiece);
				const Bitboard mask = Bitboard::setMaskBB(ksq) | Bitboard::setMaskBB(ijsq);
				Bitboard jitoBB = mask.notThisAnd(Position::attacksFrom(jipt, jicolor, jisq, mask));
				while (jitoBB.isNot0()) {
					Square jito = jitoBB.firstOneFromI9();
					Square ijsq_tmp = ijsq;
					assert(ksq <= E1);
					if (UtilSquare::ToFile(ksq) == FileE) {
						if (E1 < ijsq_tmp) {
							ij = Evaluation01::inverseFileIndexOnBoard(ij);
							ijsq_tmp = UtilSquare::InverseFile(ijsq_tmp);
							jito = UtilSquare::InverseFile(jito);
						}
						else if (UtilSquare::ToFile(ijsq_tmp) == FileE)
							jito = UtilSquare::InverseFile(jito);
					}
					const Rank ijrank = UtilSquare::ToRank(ijsq_tmp);
					const File ijfile = UtilSquare::ToFile(ijsq_tmp);
					const int distance = UtilSquare::GetSquareDistance(jisq, jito);
#if defined EVAL_PHASE3
					ret[retIdx++] = std::make_pair(&kpps.kpe[ksq][ij][jicolor][jito] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
					ret[retIdx++] = std::make_pair(&kpps.xpe[UtilSquare::ToFile(ksq)][ij][jicolor][jito] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif
					const Rank jitorank = UtilSquare::ToRank(jito);
					const File jitofile = UtilSquare::ToFile(jito);
					{
						int ij_tmp = ij;
						int jito_tmp = jito;
						if (FileE < ijfile) {
							ij_tmp = Evaluation01::inverseFileIndexOnBoard(ij_tmp);
							jito_tmp = UtilSquare::InverseFile(jito);
						}
						else if (FileE == ijfile && FileE < jitofile)
							jito_tmp = UtilSquare::InverseFile(jito);

#if defined EVAL_PHASE3
						ret[retIdx++] = std::make_pair(
							&kpps.ype[UtilSquare::ToRank(ksq)][ij_tmp][jicolor][jito_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4)
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
					ret[retIdx++] = std::make_pair(&kpps.r_kpe_b[ijpiece][R_Mid + diff_file_kij][R_Mid + krank - ijrank][jicolor][R_Mid + diff_file_kjito][R_Mid + krank - jitorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
					ret[retIdx++] = std::make_pair(&kpps.r_pe_b[ijpiece][jicolor][R_Mid + -abs(ijfile - jitofile)][R_Mid + ijrank - jitorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 4));
#endif

					int ij_tmp = ij;
					if (FileE < ijfile) {
						ij_tmp = Evaluation01::inverseFileIndexOnBoard(ij_tmp);
						jito = UtilSquare::InverseFile(jito);
					}
					else if (FileE == ijfile && E1 < jito) {
						jito = UtilSquare::InverseFile(jito);
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
				const Rank krank = UtilSquare::ToRank(ksq);
				const File kfile = UtilSquare::ToFile(ksq);
				auto color = [](int ij) {
					const int ijbegin = Evaluation01::kppIndexBegin(ij);
					const Piece ijpiece = g_kppBoardIndexStartToPiece.value(ijbegin);
					const Color ijcolor = UtilPiece::ToColor(ijpiece);
					return ijcolor;
				};
				if (color(j) < color(i))
					std::swap(i, j);
				const int ibegin = Evaluation01::kppIndexBegin(i);
				const int jbegin = Evaluation01::kppIndexBegin(j);
				const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
				const Piece jpiece = g_kppBoardIndexStartToPiece.value(jbegin);
				const Square isq = static_cast<Square>(i - ibegin);
				const Square jsq = static_cast<Square>(j - jbegin);

				const Color icolor = UtilPiece::ToColor(ipiece);
				const Color jcolor = UtilPiece::ToColor(jpiece);
				const PieceType ipt = UtilPiece::ToPieceType(ipiece);
				const PieceType jpt = UtilPiece::ToPieceType(jpiece);
				const Bitboard imask = Bitboard::setMaskBB(ksq) | Bitboard::setMaskBB(jsq);
				const Bitboard jmask = Bitboard::setMaskBB(ksq) | Bitboard::setMaskBB(isq);
				Bitboard itoBB = imask.notThisAnd(Position::attacksFrom(jpt, icolor, isq, imask));
				Bitboard jtoBB = jmask.notThisAnd(Position::attacksFrom(jpt, jcolor, jsq, jmask));
				while (itoBB.isNot0()) {
					const Square ito = itoBB.firstOneFromI9();
					const int itodistance = UtilSquare::GetSquareDistance(isq, ito);
					Bitboard jtoBB_tmp = jtoBB;
					while (jtoBB_tmp.isNot0()) {
						const Square jto = jtoBB_tmp.firstOneFromI9();
						const int jtodistance = UtilSquare::GetSquareDistance(jsq, jto);
						const int distance = itodistance + jtodistance - 1;
						{
							Square ito_tmp = ito;
							Square jto_tmp = jto;
							if (kfile == FileE) {
								if (icolor == jcolor) {
									if (std::min(UtilSquare::InverseFile(ito_tmp), UtilSquare::InverseFile(jto_tmp)) < std::min(ito_tmp, jto_tmp)) {
										ito_tmp = UtilSquare::InverseFile(ito_tmp);
										jto_tmp = UtilSquare::InverseFile(jto_tmp);
									}
									if (jto_tmp < ito_tmp)
										std::swap(ito_tmp, jto_tmp);
								}
								else {
									if (E1 < ito_tmp) {
										ito_tmp = UtilSquare::InverseFile(ito_tmp);
										jto_tmp = UtilSquare::InverseFile(jto_tmp);
									}
									else if (UtilSquare::ToFile(ito_tmp) == FileE && E1 < jto_tmp)
										jto_tmp = UtilSquare::InverseFile(jto_tmp);
								}
							}
							else if (icolor == jcolor && jto_tmp < ito_tmp)
								std::swap(ito_tmp, jto_tmp);
#if defined EVAL_PHASE1
							// ee は数が多くなる為、重みを更に小さくする。
							ret[retIdx++] = std::make_pair(&kpps.kee[ksq][icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
							ret[retIdx++] = std::make_pair(&kpps.xee[kfile][icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
							File diff_file_kito = kfile - UtilSquare::ToFile(ito_tmp);
							bool kfile_itofile_is_inversed = false;
							if (0 < diff_file_kito) {
								diff_file_kito = -diff_file_kito;
								kfile_itofile_is_inversed = true;
							}
							File diff_file_kjto =
								static_cast<File>(diff_file_kito == static_cast<File>(0) ? -abs(kfile - UtilSquare::ToFile(jto_tmp)) :
									kfile_itofile_is_inversed ? UtilSquare::ToFile(jto_tmp) - kfile : kfile - UtilSquare::ToFile(jto_tmp));
							Rank diff_rank_kito = krank - UtilSquare::ToRank(ito_tmp);
							Rank diff_rank_kjto = krank - UtilSquare::ToRank(jto_tmp);
							ColorFileRank iColorFileRank = ColorFileRank(icolor, diff_file_kito, diff_rank_kito);
							ColorFileRank jColorFileRank = ColorFileRank(jcolor, diff_file_kjto, diff_rank_kjto);
							if (jColorFileRank.GetOrder() < iColorFileRank.GetOrder())
								std::swap(iColorFileRank, jColorFileRank);
#if defined EVAL_PHASE1
							ret[retIdx++] = std::make_pair(&kpps.r_kee[iColorFileRank.color][R_Mid + iColorFileRank.file][R_Mid + iColorFileRank.rank][jColorFileRank.color][R_Mid + jColorFileRank.file][R_Mid + jColorFileRank.rank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
						}
						Square ito_tmp = ito;
						Square jto_tmp = jto;
						if (icolor == jcolor) {
							if (std::min(UtilSquare::InverseFile(ito_tmp), UtilSquare::InverseFile(jto_tmp)) < std::min(ito_tmp, jto_tmp)) {
								ito_tmp = UtilSquare::InverseFile(ito_tmp);
								jto_tmp = UtilSquare::InverseFile(jto_tmp);
							}
							if (jto_tmp < ito_tmp)
								std::swap(ito_tmp, jto_tmp);
						}
						else {
							if (E1 < ito_tmp) {
								ito_tmp = UtilSquare::InverseFile(ito_tmp);
								jto_tmp = UtilSquare::InverseFile(jto_tmp);
							}
							else if (UtilSquare::ToFile(ito_tmp) == FileE && E1 < jto_tmp)
								jto_tmp = UtilSquare::InverseFile(jto_tmp);
						}
#if defined EVAL_PHASE1
						ret[retIdx++] = std::make_pair(&kpps.ee[icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
						ret[retIdx++] = std::make_pair(&kpps.yee[krank][icolor][ito_tmp][jcolor][jto_tmp] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
						const File itofile = UtilSquare::ToFile(ito_tmp);
						const Rank itorank = UtilSquare::ToRank(ito_tmp);
						const File jtofile = UtilSquare::ToFile(jto_tmp);
						const Rank jtorank = UtilSquare::ToRank(jto_tmp);
#if defined EVAL_PHASE1
						ret[retIdx++] = std::make_pair(&kpps.r_ee[icolor][jcolor][R_Mid + abs(-itofile - jtofile)][R_Mid + itorank - jtorank] - GetKppOneArrayFirst(0), MaxWeight() >> (distance + 6));
#endif
					}
				}
			};
#if defined EVAL_PHASE1
			ee_func(ksq, i, j);
#endif

			if (ifile == FileE) {
				// ppに関してiが5筋なのでjだけ左右反転しても構わない。
				j = Evaluation01::inverseFileIndexIfLefterThanMiddle(j);
				if (j < i) std::swap(i, j);
			}
			else if ((E1 < isq)
				|| (ibegin == jbegin && UtilSquare::InverseFile(jsq) < isq))
			{
				// ppに関してiを左右反転するのでjも左右反転する。
				i = Evaluation01::inverseFileIndexOnBoard(i);
				j = Evaluation01::inverseFileIndexOnBoard(j);
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
		assert(retIdx <= KPPIndicesMax);
	}


	void CreateKkpIndices(std::pair<ptrdiff_t, int> ret[KKPIndicesMax], Square ksq0, Square ksq1, int i) {
		int retIdx = 0;
		if (ksq0 == ksq1) {
			ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
			assert(retIdx <= KKPIndicesMax);
			return;
		}
		auto kp_func = [this, &retIdx, &ret](Square ksq, int i, int sign) {
			if (E1 < ksq) {
				ksq = UtilSquare::InverseFile(ksq);
				i = Evaluation01::inverseFileIndexIfOnBoard(i);
			}
			else if (UtilSquare::ToFile(ksq) == FileE)
				i = Evaluation01::inverseFileIndexIfLefterThanMiddle(i);
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
					const int ibegin = Evaluation01::kppIndexBegin(i);
					const Square isq = static_cast<Square>(i - ibegin);
					const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
#if defined EVAL_PHASE2
					ret[retIdx++] = std::make_pair(sign*(&kkps.r_kp_b[ipiece][R_Mid + -abs(UtilSquare::ToFile(ksq) - UtilSquare::ToFile(isq))][R_Mid + UtilSquare::ToRank(ksq) - UtilSquare::ToRank(isq)] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif

#if defined EVAL_PHASE1
					const PieceType ipt = UtilPiece::ToPieceType(ipiece);
					const Color icolor = UtilPiece::ToColor(ipiece);
					Bitboard itoBB = Bitboard::setMaskBB(ksq).notThisAnd(
						Position::attacksFrom(ipt, icolor, isq, Bitboard::setMaskBB(ksq)));
					while (itoBB.isNot0()) {
						Square ito = itoBB.firstOneFromI9();
						const int distance = UtilSquare::GetSquareDistance(isq, ito);
						ret[retIdx++] = std::make_pair(sign*(&kkps.r_ke[icolor][R_Mid + -abs(UtilSquare::ToFile(ksq) - UtilSquare::ToFile(ito))][R_Mid + UtilSquare::ToRank(ksq) - UtilSquare::ToRank(ito)] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
					}
#endif
				}
			};
			r_kp_func(ksq, i, sign);
#if defined EVAL_PHASE1
			if (f_pawn <= i) {
				const int ibegin = Evaluation01::kppIndexBegin(i);
				const Square isq = static_cast<Square>(i - ibegin);
				const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
				const PieceType ipt = UtilPiece::ToPieceType(ipiece);
				const Color icolor = UtilPiece::ToColor(ipiece);

				Bitboard itoBB = Bitboard::setMaskBB(ksq).notThisAnd(
					Position::attacksFrom(ipt, icolor, isq, Bitboard::setMaskBB(ksq)));
				while (itoBB.isNot0()) {
					Square ito = itoBB.firstOneFromI9();
					const int distance = UtilSquare::GetSquareDistance(isq, ito);
					if (UtilSquare::ToFile(ksq) == FileE && E1 < ito)
						ito = UtilSquare::InverseFile(ito);
					ret[retIdx++] = std::make_pair(sign*(&kkps.ke[ksq][icolor][ito] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
				}
			}
#endif
		};

		kp_func(ksq0, i, 1);
		{
			const int begin = Evaluation01::kppIndexBegin(i);
			const int opp_begin = Evaluation01::kppIndexToOpponentBegin(i);
			const int tmp_i = (begin < fe_hand_end ? opp_begin + (i - begin) : opp_begin + UtilSquare::Inverse(static_cast<Square>(i - begin)));
			kp_func(UtilSquare::Inverse(ksq1), tmp_i, -1);
		}

		int sign = 1;
		if (!Evaluation01::kppIndexIsBlack(i)) {
			const Square tmp = ksq0;
			ksq0 = UtilSquare::Inverse(ksq1);
			ksq1 = UtilSquare::Inverse(tmp);
			const int ibegin = Evaluation01::kppIndexBegin(i);
			const int opp_ibegin = Evaluation01::kppWhiteIndexToBlackBegin(i);
			i = opp_ibegin + (i < fe_hand_end ? i - ibegin : UtilSquare::Inverse(static_cast<Square>(i - ibegin)));
			sign = -1;
		}
		if (E1 < ksq0) {
			ksq0 = UtilSquare::InverseFile(ksq0);
			ksq1 = UtilSquare::InverseFile(ksq1);
			i = Evaluation01::inverseFileIndexIfOnBoard(i);
		}
		else if (UtilSquare::ToFile(ksq0) == FileE && E1 < ksq1) {
			ksq1 = UtilSquare::InverseFile(ksq1);
			i = Evaluation01::inverseFileIndexIfOnBoard(i);
		}
		else if (UtilSquare::ToFile(ksq0) == FileE && UtilSquare::ToFile(ksq1) == FileE) {
			i = Evaluation01::inverseFileIndexIfLefterThanMiddle(i);
		}
#if defined EVAL_PHASE4
		ret[retIdx++] = std::make_pair(sign*(&kkps.kkp[ksq0][ksq1][i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif

#if defined EVAL_PHASE1 || defined EVAL_PHASE3
		const Rank diff_rank_k0k1 = UtilSquare::ToRank(ksq0) - UtilSquare::ToRank(ksq1);
		File diff_file_k0k1 = UtilSquare::ToFile(ksq0) - UtilSquare::ToFile(ksq1);
		if (i < fe_hand_end) {
			if (0 < diff_file_k0k1)
				diff_file_k0k1 = -diff_file_k0k1;
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(sign*(&kkps.r_kkp_h[R_Mid + diff_file_k0k1][R_Mid + diff_rank_k0k1][i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif
		}
		else {
			const int ibegin = Evaluation01::kppIndexBegin(i);
			const Piece ipiece = g_kppBoardIndexStartToPiece.value(ibegin);
			Square isq = static_cast<Square>(i - ibegin);
			const Rank diff_rank_k0i = UtilSquare::ToRank(ksq0) - UtilSquare::ToRank(isq);
			File diff_file_k0i = UtilSquare::ToFile(ksq0) - UtilSquare::ToFile(isq);

			const Color icolor = UtilPiece::ToColor(ipiece);
			const PieceType ipt = UtilPiece::ToPieceType(ipiece);
			const Bitboard mask = Bitboard::setMaskBB(ksq0) | Bitboard::setMaskBB(ksq1);
			Bitboard itoBB = mask.notThisAnd(Position::attacksFrom(ipt, icolor, isq, mask));
			while (itoBB.isNot0()) {
				Square ito = itoBB.firstOneFromI9();
				const int distance = UtilSquare::GetSquareDistance(isq, ito);
				if (UtilSquare::ToFile(ksq0) == FileE && UtilSquare::ToFile(ksq1) == FileE && E1 < ito)
					ito = UtilSquare::InverseFile(ito);
#if defined EVAL_PHASE3
				ret[retIdx++] = std::make_pair(sign*(&kkps.kke[ksq0][ksq1][icolor][ito] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
#endif
#if defined EVAL_PHASE1
				File diff_file_k0k1_tmp = diff_file_k0k1;
				File diff_file_k0ito = UtilSquare::ToFile(ksq0) - UtilSquare::ToFile(ito);
				Rank diff_rank_k0ito = UtilSquare::ToRank(ksq0) - UtilSquare::ToRank(ito);
				if (0 < diff_file_k0k1_tmp) {
					diff_file_k0k1_tmp = -diff_file_k0k1_tmp;
					diff_file_k0ito = -diff_file_k0ito;
				}
				else if (0 == diff_file_k0k1_tmp && 0 < diff_file_k0ito)
					diff_file_k0ito = -diff_file_k0ito;
				ret[retIdx++] = std::make_pair(sign*(&kkps.r_kke[R_Mid + diff_file_k0k1_tmp][R_Mid + diff_rank_k0k1][icolor][R_Mid + diff_file_k0ito][R_Mid + diff_rank_k0ito] - GetKkpOneArrayFirst(0)), MaxWeight() >> (distance + 4));
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
			ret[retIdx++] = std::make_pair(sign*(&kkps.r_kkp_b[R_Mid + diff_file_k0k1][R_Mid + diff_rank_k0k1][ipiece][R_Mid + diff_file_k0i][R_Mid + diff_rank_k0i] - GetKkpOneArrayFirst(0)), MaxWeight());
#endif
		}
#endif
		ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
		assert(retIdx <= KKPIndicesMax);
	}


	void CreateKkIndices(std::pair<ptrdiff_t, int> ret[KKIndicesMax], Square ksq0, Square ksq1) {
		int retIdx = 0;
#if defined EVAL_PHASE1
		ret[retIdx++] = std::make_pair(&kks.k[std::min(ksq0, UtilSquare::InverseFile(ksq0))] - GetKkOneArrayFirst(0), MaxWeight());
		ret[retIdx++] = std::make_pair(-(&kks.k[std::min(UtilSquare::Inverse(ksq1), UtilSquare::InverseFile(UtilSquare::Inverse(ksq1)))] - GetKkOneArrayFirst(0)), MaxWeight());
#endif

		auto kk_func = [this, &retIdx, &ret](Square ksq0, Square ksq1, int sign) {
			{
				// 常に ksq0 < ksq1 となるテーブルにアクセスする為の変換
				const Square ksq0Arr[] = {
					ksq0,
					UtilSquare::InverseFile(ksq0),
				};
				const Square ksq1Arr[] = {
					UtilSquare::Inverse(ksq1),
					UtilSquare::Inverse(UtilSquare::InverseFile(ksq1)),
				};
				auto ksq0ArrIdx = std::min_element(std::begin(ksq0Arr), std::end(ksq0Arr)) - std::begin(ksq0Arr);
				auto ksq1ArrIdx = std::min_element(std::begin(ksq1Arr), std::end(ksq1Arr)) - std::begin(ksq1Arr);
				if (ksq0Arr[ksq0ArrIdx] <= ksq1Arr[ksq1ArrIdx]) {
					ksq0 = ksq0Arr[ksq0ArrIdx];
					ksq1 = UtilSquare::Inverse(ksq1Arr[ksq0ArrIdx]);
				}
				else {
					sign = -sign; // ksq0 と ksq1 を入れ替えるので符号反転
					ksq0 = ksq1Arr[ksq1ArrIdx];
					ksq1 = UtilSquare::Inverse(ksq0Arr[ksq1ArrIdx]);
				}
			}
			const File kfile0 = UtilSquare::ToFile(ksq0);
			const Rank krank0 = UtilSquare::ToRank(ksq0);
			const File kfile1 = UtilSquare::ToFile(ksq1);
			const Rank krank1 = UtilSquare::ToRank(ksq1);
#if defined EVAL_PHASE3
			ret[retIdx++] = std::make_pair(sign*(&kks.kk[ksq0][ksq1] - GetKkOneArrayFirst(0)), MaxWeight());
#endif
#if defined EVAL_PHASE2
			ret[retIdx++] = std::make_pair(sign*(&kks.r_kk[R_Mid + kfile0 - kfile1][R_Mid + krank0 - krank1] - GetKkOneArrayFirst(0)), MaxWeight());
#endif
			assert(ksq0 <= E1);
			assert(kfile0 - kfile1 <= 0);
		};
		kk_func(ksq0, ksq1, 1);
		kk_func(UtilSquare::Inverse(ksq1), UtilSquare::Inverse(ksq0), -1);
		ret[retIdx++] = std::make_pair(std::numeric_limits<ptrdiff_t>::max(), MaxWeight());
		assert(retIdx <= KKIndicesMax);
	}


	void clear() { memset(this, 0, sizeof(*this)); } // float 型とかだと規格的に 0 は保証されなかった気がするが実用上問題ないだろう。


};

