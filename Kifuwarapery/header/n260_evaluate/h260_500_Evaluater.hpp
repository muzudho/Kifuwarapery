#pragma once

#include "../../header/n260_evaluate/h260_400_EvaluaterBase.hpp"


struct Evaluater : public EvaluaterBase<std::array<s16, 2>, std::array<s32, 2>, std::array<s32, 2> > {
	// �T�����ɎQ�Ƃ���]���֐��e�[�u��
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
		// �������ꂽ�]���֐��o�C�i��������΂�������g���B
		if (Synthesized) {
			if (readSynthesized(dirName))
			{
				SYNCCOUT << "(init 1/4)Use synthesized file!" << SYNCENDL;
				return;
			}
		}
		clear();

#if defined(MODE_EVAL)
		SYNCCOUT << "(init 2/4) readSomeSynthesized!" << SYNCENDL;
#endif
		Evaluater::readSomeSynthesized(dirName);

#if defined(MODE_EVAL)
		SYNCCOUT << "(init 3/4) (long time)read bins! dir=" << dirName << SYNCENDL;
#endif
		Evaluater::ReadBins(dirName);

#if defined(MODE_EVAL)
		SYNCCOUT << "(init 4/4) (long time)setEvaluate!" << SYNCENDL;
#endif
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
	// �t�F�[�Y�P�̒�`��������
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
	// �t�F�[�Y�P�̒�`�����܂�
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


	// �t�F�[�Y�P�`�S�ɓW�J����܂��B
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

	// .bin �t�@�C����ǂݍ��ށB
	void ReadBins(const std::string& dirName) {

		// �֐���`��������
#define FOO(x) {														\
			std::ifstream ifs((addSlashIfNone(dirName) + #x ".bin").c_str(), std::ios::binary); \
			ifs.read(reinterpret_cast<char*>(x), sizeof(x));			\
		}
		// �֐���`�����܂�

		// �t�F�[�Y�P�`�S�ɓW�J����܂��B
		READ_BASE_EVAL;

		// �֐���`��p��
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

	bool WriteKppCache3Files(const std::string& dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);

	bool ReadKppCache2Files(const std::string& dirName, int k1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);
	bool ReadKppCache3Files(const std::string& dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);

	bool SynthesizeKppCache3ToCache2(const std::string& dirName, int k1);

	std::string GetKppCache2FilePath(const std::string& dirName, int k1);
	std::string GetKppCache3FilePath(const std::string& dirName, int k1, int p1);

private:
	void WriteKppCache3FilesBody(const std::string& cache3Filepath, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end]);

public:

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
#if defined(MODE_EVAL)
		SYNCCOUT << "(^q^)KPP!" << SYNCENDL;
#endif
		// KPP
		{
			#ifdef _OPENMP
			#pragma omp for
			#endif

			// 81 x 1548 x 1548 �̃��[�v�B

			// �Ֆʂ� [0, 80] �̐����� index �ŕ\��
			// I9 = 1��, I1 = 1��, A1 = 9��
			//
			// A9, B9, C9, D9, E9, F9, G9, H9, I9,
			// A8, B8, C8, D8, E8, F8, G8, H8, I8,
			// A7, B7, C7, D7, E7, F7, G7, H7, I7,
			// A6, B6, C6, D6, E6, F6, G6, H6, I6,
			// A5, B5, C5, D5, E5, F5, G5, H5, I5,
			// A4, B4, C4, D4, E4, F4, G4, H4, I4,
			// A3, B3, C3, D3, E3, F3, G3, H3, I3,
			// A2, B2, C2, D2, E2, F2, G2, H2, I2,
			// A1, B1, C1, D1, E1, F1, G1, H1, I1,

			// �悭�g���ʂ̈ʒu���珇�� KPP ��ݒ肵�Ă����i�O���O�j
			enum Square squares[] = {
				A5, // �T��
				B8, // �W��
				B2, // �Q��
				A9, // �X��
				A1, // �P��
				A6, // �U��
				B4, // �S��
				A7, // �V��
				B3, // �R��
				B6, // �U��
				B7, // �V��
				B5, // �T��
				A3, // �R��
				C8, // �W�� �V��t���Z
				B1, // �P��
				A4, // �S��
				B9, // �X��
				A8, // �W��
				A2, // �Q��
				C9,	    C7, C6, C5, C4, C3, C2, C1,// 3�i��
				D9,	D8, D7, D6, D5, D4, D3, D2, D1,
				E9,	E8, E7, E6, E5, E4, E3, E2, E1,
				F9,	F8, F7, F6, F5, F4, F3, F2, F1,
				G9,	G8, G7, G6, G5, G4, G3, G2, G1,
				H9,	H8, H7, H6, H5, H4, H3, H2, H1,
				I9, I8, I7, I6, I5, I4, I3, I2, I1,
			};

			// OpenMP�Ή������牽�̂� k1 �� Square �^�ɂ���� ++k1 ����`����Ă��Ȃ��ăR���p�C���G���[�ɂȂ�B
			//for (int k1 = I9; k1 < SquareNum; ++k1) {
			//}
			int size = sizeof(squares) / sizeof(squares[0]);
			for (int o = 0; o < size; o++)//order
			{
				int k1 = squares[o];//�ʂ̈ʒu

				// �ł���΁ACache3 ���ACache2 �ɓ������܂��B
				this->SynthesizeKppCache3ToCache2(dirName, k1);

#if defined(MODE_EVAL)
				SYNCCOUT << "(^q^)Go Cache2!" << SYNCENDL;
#endif

				if (this->ReadKppCache2Files(dirName, k1, KPP))
				{
					// ���ԃt�@�C������Ǎ������B
#if defined(MODE_EVAL)
					SYNCCOUT << "(^q^)KPP: k1=" << std::to_string(k1) << "(" << std::to_string(o) << "/" << std::to_string(SquareNum) << ") loaded." << SYNCENDL;
#endif
				}
				else
				{
#if defined(MODE_EVAL)
					SYNCCOUT << "(^q^)KPP: k1=" << std::to_string(k1) << "/" << std::to_string(SquareNum) << SYNCENDL;
#endif

					// indices �͍X�� for ���[�v�̊O���ɒu���������AOpenMP �g���Ă���ƃA�N�Z�X�����������Ȃ̂Ń��[�v�̒��ɒu���B
					std::pair<ptrdiff_t, int> indices[KPPIndicesMax];
					for (int p1 = 0; p1 < fe_end; ++p1) {

						//SYNCCOUT << "(^q^)ReadKppCache3Files!" << SYNCENDL;
						if (this->ReadKppCache3Files(dirName, k1, p1, KPP))
						{
#if defined(MODE_EVAL)
							SYNCCOUT << "(^q^)KPP: p1=" << std::to_string(p1) << "/" << std::to_string(fe_end) << " loaded." << SYNCENDL;
#endif
							// ���ԃt�@�C������Ǎ������B
						}
						else
						{
#if !defined(SKIP_NOT_EXIST_EVAL_FILE)
							// �W�v�J�n�B

							for (int p2 = 0; p2 < fe_end; ++p2) {

								this->CreateKppIndices(indices, static_cast<Square>(k1), p1, p2);
								std::array<s64, 2> sum = { {} };
								FOO(indices, GetKppOneArrayFirst, sum);
								KPP[k1][p1][p2] += sum;
							}
							// �i�O���O�j�����Ńt�@�C�����쐬�������Ǝv���񂾂���
							// �^�� std::array<s16, 2> Evaluater::KPP[81][1548][1548];
							// short�^(2byte?) �v�f�� 2 �̔z��B
							// 1548 x 1548 x 2byte �T�C�Y�̃o�C�i���E�t�@�C���� 81 �� KPP �z��ɂȂ�͂����i�O���O�j
							// �t�@�C������ �uKKP[����].obj�v�łǂ��������H�i�O���O�j
							if (this->WriteKppCache3Files(dirName, k1, p1, KPP))
							{
#if defined(MODE_EVAL)
								SYNCCOUT << "(^q^)KPP: p1=" << std::to_string(p1) << "/" << std::to_string(fe_end) << " writed!" << SYNCENDL;
#endif
							}
							else
							{
								// �G���[
								goto gt_EndKPP;
							}
#endif
						}
					}
				}
			}
		gt_EndKPP:
			;
		}
#endif

#ifndef SKIP_LONG_LONG_TIME_EVAL_KKP
#if defined(MODE_EVAL)
		SYNCCOUT << "(^q^)KKP!" << SYNCENDL;
#endif
		// KKP
		{
			#ifdef _OPENMP
			#pragma omp for
			#endif
			for (int ksq0 = I9; ksq0 < SquareNum; ++ksq0) {
#if defined(MODE_EVAL)
				SYNCCOUT << "(^q^)KKP: ksq0=" << std::to_string(ksq0) << "/" << std::to_string(SquareNum) << SYNCENDL;
#endif

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
#if defined(MODE_EVAL)
		SYNCCOUT << "(^q^)KK!" << SYNCENDL;
#endif
		// KK
		{
			#ifdef _OPENMP
			#pragma omp for
			#endif
			for (int ksq0 = I9; ksq0 < SquareNum; ++ksq0) {
#if defined(MODE_EVAL)
				SYNCCOUT << "(^q^)KK: ksq0=" << std::to_string(ksq0) << "/" << std::to_string(SquareNum) << SYNCENDL;
#endif

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
