#pragma once

#include "../../header/n260_evaluate/h260_400_EvaluaterBase.hpp"


struct Evaluater : public EvaluaterBase<std::array<s16, 2>, std::array<s32, 2>, std::array<s32, 2> > {
	// 探索時に参照する評価関数テーブル
	static std::array<s16, 2> KPP[SquareNum][fe_end][fe_end];
	static std::array<s32, 2> KKP[SquareNum][SquareNum][fe_end];
	static std::array<s32, 2> KK[SquareNum][SquareNum];
#if defined USE_K_FIX_OFFSET
	static const s32 K_Fix_Offset[SquareNum];
#endif


	void clear() { memset(this, 0, sizeof(*this)); }


	static std::string addSlashIfNone(const std::string& str) {
		std::string ret = str;
		if (ret == "")
			ret += ".";
		if (ret.back() != '/')
			ret += "/";
		return ret;
	}


	void init(const std::string& dirName, const bool Synthesized) {
		// 合成された評価関数バイナリがあればそちらを使う。
		if (Synthesized) {
			if (readSynthesized(dirName))
				return;
		}
		clear();

		SYNCCOUT << "(^q^)B: readSomeSynthesized!" << SYNCENDL;
		Evaluater::readSomeSynthesized(dirName);

		SYNCCOUT << "(^q^)C: (long time)read! dir=" << dirName << SYNCENDL;
		Evaluater::ReadBins(dirName);

		SYNCCOUT << "(^q^)D: (long time)setEvaluate!" << SYNCENDL;
		Evaluater::SetEvaluate(dirName);
	}


	static bool readSynthesized(const std::string& dirName) {
#define FOO(x) {														\
			std::ifstream ifs((addSlashIfNone(dirName) + #x "_synthesized.bin").c_str(), std::ios::binary); \
			if (ifs) ifs.read(reinterpret_cast<char*>(x), sizeof(x));	\
			else     return false;										\
		}

		//ALL_SYNTHESIZED_EVAL
#ifndef SKIP_LONG_LONG_TIME_EVAL_KPP
		FOO(KPP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KKP
		FOO(KKP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KK
		FOO(KK);
#endif

#undef FOO
		return true;
	}


	static void writeSynthesized(const std::string& dirName) {
#define FOO(x) {														\
			std::ofstream ofs((addSlashIfNone(dirName) + #x "_synthesized.bin").c_str(), std::ios::binary); \
			ofs.write(reinterpret_cast<char*>(x), sizeof(x));			\
		}

		//ALL_SYNTHESIZED_EVAL
#ifndef SKIP_LONG_LONG_TIME_EVAL_KPP
		FOO(KPP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KKP
		FOO(KKP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KK
		FOO(KK);
#endif

#undef FOO
	}


	static void readSomeSynthesized(const std::string& dirName) {
#define FOO(x) {														\
			std::ifstream ifs((addSlashIfNone(dirName) + #x "_some_synthesized.bin").c_str(), std::ios::binary); \
			if (ifs) ifs.read(reinterpret_cast<char*>(x), sizeof(x));	\
			else     memset(x, 0, sizeof(x));							\
		}

		//ALL_SYNTHESIZED_EVAL
#ifndef SKIP_LONG_LONG_TIME_EVAL_KPP
		FOO(KPP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KKP
		FOO(KKP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KK
		FOO(KK);
#endif

#undef FOO
	}


	static void writeSomeSynthesized(const std::string& dirName) {
#define FOO(x) {														\
			std::ofstream ofs((addSlashIfNone(dirName) + #x "_some_synthesized.bin").c_str(), std::ios::binary); \
			ofs.write(reinterpret_cast<char*>(x), sizeof(x));			\
		}

		//ALL_SYNTHESIZED_EVAL
#ifndef SKIP_LONG_LONG_TIME_EVAL_KPP
		FOO(KPP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KKP
		FOO(KKP);
#endif
#ifndef SKIP_LONG_LONG_TIME_EVAL_KK
		FOO(KK);
#endif

#undef FOO
	}


#if defined EVAL_PHASE1
	// フェーズ１の定義ここから
#define BASE_PHASE1 {								\
		FOO(kpps.kee);								\
		FOO(kpps.r_kpe_b);							\
		FOO(kpps.r_kpe_h);							\
		FOO(kpps.r_kee);							\
		FOO(kpps.xee);								\
		FOO(kpps.yee);								\
		FOO(kpps.pe);								\
		FOO(kpps.ee);								\
		FOO(kpps.r_pe_b);							\
		FOO(kpps.r_pe_h);							\
		FOO(kpps.r_ee);								\
		FOO(kkps.ke);								\
		FOO(kkps.r_kke);							\
		FOO(kkps.r_ke);								\
		FOO(kks.k);									\
	}
	// フェーズ１の定義ここまで
#else
#define BASE_PHASE1
#endif


#if defined EVAL_PHASE2
#define BASE_PHASE2 {								\
		FOO(kpps.r_pp_bb);							\
		FOO(kpps.r_pp_hb);							\
		FOO(kkps.r_kp_b);							\
		FOO(kkps.r_kp_h);							\
		FOO(kks.r_kk);								\
	}
#else
#define BASE_PHASE2
#endif


#if defined EVAL_PHASE3
#define BASE_PHASE3 {								\
		FOO(kpps.r_kpp_bb);							\
		FOO(kpps.r_kpp_hb);							\
		FOO(kpps.pp);								\
		FOO(kpps.kpe);								\
		FOO(kpps.xpe);								\
		FOO(kpps.ype);								\
		FOO(kkps.kp);								\
		FOO(kkps.r_kkp_b);							\
		FOO(kkps.r_kkp_h);							\
		FOO(kkps.kke);								\
		FOO(kks.kk);								\
	}
#else
#define BASE_PHASE3
#endif


#if defined EVAL_PHASE4
#define BASE_PHASE4 {								\
		FOO(kpps.kpp);								\
		FOO(kpps.xpp);								\
		FOO(kpps.ypp);								\
		FOO(kkps.kkp);								\
	}
#else
#define BASE_PHASE4
#endif


	// フェーズ１～４に展開されます。
#define READ_BASE_EVAL {						\
		BASE_PHASE1;							\
		BASE_PHASE2;							\
		BASE_PHASE3;							\
		BASE_PHASE4;							\
	}

#define WRITE_BASE_EVAL {						\
		BASE_PHASE1;							\
		BASE_PHASE2;							\
		BASE_PHASE3;							\
		BASE_PHASE4;							\
	}

	// .bin ファイルを読み込む。
	void ReadBins(const std::string& dirName) {

		// 関数定義ここから
#define FOO(x) {														\
			std::ifstream ifs((addSlashIfNone(dirName) + #x ".bin").c_str(), std::ios::binary); \
			ifs.read(reinterpret_cast<char*>(x), sizeof(x));			\
		}
		// 関数定義ここまで

		// フェーズ１～４に展開されます。
		READ_BASE_EVAL;

		// 関数定義を廃棄
#undef FOO
	}


	void write(const std::string& dirName) {
#define FOO(x) {														\
			std::ofstream ofs((addSlashIfNone(dirName) + #x ".bin").c_str(), std::ios::binary); \
			ofs.write(reinterpret_cast<char*>(x), sizeof(x));			\
		}
		WRITE_BASE_EVAL;
#undef FOO
	}

#undef READ_BASE_EVAL
#undef WRITE_BASE_EVAL


	void WriteKppPartFile(const std::string& dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]) {
		std::ofstream ofs((addSlashIfNone(dirName) + "obj/Kpp[" + std::to_string( k1) + "]/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj").c_str(), std::ios::binary);

		for (int p2 = 0; p2 < fe_end; ++p2) {
			for (int z = 0; z < 2; ++z) {
				ofs << kppArray[k1][p1][p2][z];
			}
		}
	}

	bool ReadKppPartFile(const std::string& dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]) {
		std::string path = addSlashIfNone(dirName) + "obj/Kpp[" + std::to_string( k1) + "]/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";
		//SYNCCOUT << "File Search: [" << path << "]" << SYNCENDL;

		std::ifstream ifs(path.c_str(), std::ios::binary);
		if (ifs.fail()) {
			std::cerr << "Error : Could not open" << std::endl;
			//なんか終了処理を入れる
			return false;
		}

		//読み込むファイル格納用配列
		short buffer[fe_end][fe_end][2];
		ifs.read((char*)buffer, fe_end*fe_end * 2 * sizeof(short));

		for (int p2 = 0; p2 < fe_end; ++p2) {
			for (int z = 0; z < 2; ++z) {
				kppArray[k1][p1][p2][z] = buffer[p1][p2][z];
			}
		}

		return true;
	}



	void SetEvaluate(const std::string& dirName) {
#if !defined LEARN
		SYNCCOUT << "info string START setting eval table" << SYNCENDL;
#endif
#define FOO(indices, oneArray, sum)										\
		for (auto indexAndWeight : indices) {							\
			if (indexAndWeight.first == std::numeric_limits<ptrdiff_t>::max()) break; \
			if (0 <= indexAndWeight.first) {							\
				sum[0] += static_cast<s64>((*oneArray( indexAndWeight.first))[0]) * indexAndWeight.second; \
				sum[1] += static_cast<s64>((*oneArray( indexAndWeight.first))[1]) * indexAndWeight.second; \
			}															\
			else {														\
				sum[0] -= static_cast<s64>((*oneArray(-indexAndWeight.first))[0]) * indexAndWeight.second; \
				sum[1] += static_cast<s64>((*oneArray(-indexAndWeight.first))[1]) * indexAndWeight.second; \
			}															\
		}																\
		sum[0] /= MaxWeight();											\
		sum[1] /= MaxWeight() * TurnWeight();

#if defined _OPENMP
#pragma omp parallel
#endif

#ifndef SKIP_LONG_LONG_TIME_EVAL_KPP
		SYNCCOUT << "(^q^)KPP!" << SYNCENDL;
		// KPP
		//KPP[SquareNum][fe_end][fe_end] = { 0 };
		{
			#ifdef _OPENMP
			#pragma omp for
			#endif

			// 81 x 1548 x 1548 のループ。

			// OpenMP対応したら何故か k1 を Square 型にすると ++k1 が定義されていなくてコンパイルエラーになる。
			for (int k1 = I9; k1 < SquareNum; ++k1) {
				SYNCCOUT << "(^q^)KPP: k1=" << k1 << "/" << SquareNum << SYNCENDL;

				// indices は更に for ループの外側に置きたいが、OpenMP 使っているとアクセス競合しそうなのでループの中に置く。
				std::pair<ptrdiff_t, int> indices[KPPIndicesMax];
				for (int p1 = 0; p1 < fe_end; ++p1) {

					//SYNCCOUT << "(^q^)ReadKppPartFile!" << SYNCENDL;
					if (this->ReadKppPartFile(dirName, k1, p1, KPP))
					{
						SYNCCOUT << "(^q^)KPP: p1=" << p1 << "/" << fe_end << " loaded." << SYNCENDL;
						// 中間ファイルから読込完了。
					}
					else
					{
						// 集計開始。

						for (int p2 = 0; p2 < fe_end; ++p2) {

							this->CreateKppIndices(indices, static_cast<Square>(k1), p1, p2);
							std::array<s64, 2> sum = { {} };
							FOO(indices, GetKppOneArrayFirst, sum);
							KPP[k1][p1][p2] += sum;
						}
						// （＾ｑ＾）ここでファイルを作成したいと思うんだぜ☆
						// 型は std::array<s16, 2> Evaluater::KPP[81][1548][1548];
						// short型(2byte?) 要素数 2 の配列。
						// 1548 x 1548 x 2byte サイズのバイナリ・ファイルが 81 個で KPP 配列になるはず☆（＾ｑ＾）
						// ファイル名は 「KKP[数字].obj」でどうだぜ☆？（＾ｑ＾）
						this->WriteKppPartFile(dirName, k1, p1, KPP);
						SYNCCOUT << "(^q^)KPP: p1=" << p1 << "/" << fe_end << " writed!" << SYNCENDL;
					}
				}
			}
		}
#endif

#ifndef SKIP_LONG_LONG_TIME_EVAL_KKP
		SYNCCOUT << "(^q^)KKP!" << SYNCENDL;
		// KKP
		{
			#ifdef _OPENMP
			#pragma omp for
			#endif
			for (int ksq0 = I9; ksq0 < SquareNum; ++ksq0) {
				SYNCCOUT << "(^q^)KKP: ksq0=" << ksq0 << "/" << SquareNum << SYNCENDL;

				std::pair<ptrdiff_t, int> indices[KKPIndicesMax];
				for (Square ksq1 = I9; ksq1 < SquareNum; ++ksq1) {
					for (int p1 = 0; p1 < fe_end; ++p1) {
						//SYNCCOUT << "(^q^)KKP: p1=" << p1 << "/" << fe_end << SYNCENDL;

						this->CreateKkpIndices(indices, static_cast<Square>(ksq0), ksq1, p1);
						std::array<s64, 2> sum = {{}};
						FOO(indices, GetKkpOneArrayFirst, sum);
						KKP[ksq0][ksq1][p1] += sum;
					}
				}
			}
		}
#endif

#ifndef SKIP_LONG_LONG_TIME_EVAL_KK
		SYNCCOUT << "(^q^)KK!" << SYNCENDL;
		// KK
		{
			#ifdef _OPENMP
			#pragma omp for
			#endif
			for (int ksq0 = I9; ksq0 < SquareNum; ++ksq0) {
				SYNCCOUT << "(^q^)KK: ksq0=" << ksq0 << "/" << SquareNum << SYNCENDL;

				std::pair<ptrdiff_t, int> indices[KKIndicesMax];
				for (Square ksq1 = I9; ksq1 < SquareNum; ++ksq1) {
					//SYNCCOUT << "(^q^)KKP: ksq1=" << ksq1 << "/" << fe_end << SYNCENDL;

					this->CreateKkIndices(indices, static_cast<Square>(ksq0), ksq1);
					std::array<s64, 2> sum = {{}};
					FOO(indices, GetKkOneArrayFirst, sum);
					KK[ksq0][ksq1][0] += sum[0] / 2;
					KK[ksq0][ksq1][1] += sum[1] / 2;
					#if defined USE_K_FIX_OFFSET
					KK[ksq0][ksq1][0] += K_Fix_Offset[ksq0] - K_Fix_Offset[inverse(ksq1)];
					#endif
				}
			}
		}
#endif //KK
		#undef FOO


#if !defined LEARN
		SYNCCOUT << "info string END setting eval table" << SYNCENDL;
#endif
	}
};
