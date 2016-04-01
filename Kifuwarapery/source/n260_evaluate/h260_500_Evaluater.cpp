#include "../../header/n080_common__/n080_200_utilProgram.hpp"
#include "../../header/n260_evaluate/h260_500_Evaluater.hpp"
#include <Windows.h>	// �t�@�C���^�f�B���N�g������p
#include <shlwapi.h>	// �t�@�C���^�f�B���N�g������p shlwapi.lib �ւ̃����N���K�v�B


// KPP�̃t�@�C������
bool Evaluater::WriteKppCache3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	bool isError = false;

#if defined(MODE_CACHE_EVAL)

	std::string dir1 = addSlashIfNone(dirName) + "obj";
	std::string dir2 = dir1 + "/Kpp[" + std::to_string(k1) + "]";
	std::string file3 = dir2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";

	SYNCCOUT << "(WriteKppCache3Files 1/9) File Search: path3=[" << file3.c_str() << "]" << SYNCENDL;

	if (!PathIsDirectoryA((LPCSTR)dir1.c_str()))
	{
		UtilProgram utilProgram;
		SYNCCOUT << "(WriteKppCache3Files 2/9)Not found directory : dir1=[" << dir1.c_str() << "]" << SYNCENDL;
		utilProgram.ShowCurrentDirectory();
		utilProgram.ErrorBegin();
		int result = CreateDirectoryA((LPCSTR)dir1.c_str(), NULL);
		utilProgram.ErrorEnd();

		if (result !=0)
		{
			SYNCCOUT << "(WriteKppCache3Files 3/9)Create directory : dir1=[" << dir1.c_str() << "]" << SYNCENDL;
		}
		else
		{
			// �G���[
			isError = true;

			std::cerr << "(WriteKppCache3Files 4/9): Can not create directory : dir1=[" << dir1.c_str() << "]" << std::endl;
			utilProgram.ShowCurrentDirectory();
		}
	}

	if (!isError && !PathIsDirectoryA((LPCSTR)dir2.c_str()))
	{
		//SYNCCOUT << "Not found directory : path2=[" << dir2 << "]" << SYNCENDL;
		if (CreateDirectoryA((LPCSTR)dir2.c_str(), NULL))
		{
			SYNCCOUT << "(WriteKppCache3Files 5/9)Create directory : dir2=[" << dir2.c_str() << "]" << SYNCENDL;
		}
		else
		{
			// �G���[
			isError = true;
			std::cerr << "(WriteKppCache3Files 6/9)error: Can not create directory : dir2=[" << dir2.c_str() << "]" << std::endl;
		}
	}

	SYNCCOUT << "(WriteKppCache3Files 7/9)" << SYNCENDL;

	// �t�@�C���͖����͂��B
	if (!isError)
	{
		this->WriteKppCache3FilesBody(dirName, k1, p1, kppArray);
		SYNCCOUT << "(WriteKppCache3Files 8/9)" << SYNCENDL;
	}
	SYNCCOUT << "(WriteKppCache3Files 9/9)" << SYNCENDL;
#endif
	return !isError;
}

void Evaluater::WriteKppCache3FilesBody(const std::string & cache3Filepath, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	std::ofstream output(cache3Filepath, std::ios::binary);

	//�������ރt�@�C���i�[�p�z��
	char buffer[2];
	int p2 = 0;
	int z = 0;
	for (;;) {

		buffer[0] = kppArray[k1][p1][p2][0];// 2byte * 1548element = 3096byte
		buffer[1] = kppArray[k1][p1][p2][1];// 2byte * 1548element = 3096byte
		output.write((char *)buffer, sizeof(s16));// 2element * 2byte * 1548element = 6192byte
												  
		z++;// �C���N�������g�B
		if (z == 2) {
			z = 0;
			p2++;// �J��オ��
			if (p2 == fe_end) {
				break;
			}
		}
	}
	//SYNCCOUT << "(^q^)Outputed!" << SYNCENDL;
}

bool Evaluater::ReadKppCache2Files(const std::string & dirName, int k1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if defined(MODE_CACHE_EVAL)
	std::string cache2Path = this->GetKppCache2FilePath(dirName, k1);

	if (!PathFileExistsA((LPCSTR)cache2Path.c_str()))
	{
		// �t�@�C����������Ȃ��̂� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
		return false;
	}

	std::ifstream ifs(cache2Path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//�Ȃ񂩏I������������
		return false;
	}
	//SYNCCOUT << "(^q^)File opened." << std::endl;

	//�ǂݍ��ރt�@�C���i�[�p�z��
	int p1 = 0;
	int p2 = 0;
	int z = 0;
	char buffer[2]; //s16 *buffer = 0;
	while (!ifs.eof()) {

		ifs.read((char*)buffer, sizeof(s16));//2byte

		kppArray[k1][p1][p2][z] = *buffer;

		// �C���N�������g�B
		z++;
		if (z == 2) {
			z = 0;
			p2++;// �J��オ��
			if (p2 == fe_end) {
				p2 = 0;
				p1++;// �J��オ��
				if (p1 == fe_end) {
					break;
				}
			}
		}
	}

	return true;
#else
	return false;
#endif
}

bool Evaluater::ReadKppCache3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if defined(MODE_CACHE_EVAL)
	std::string catch3Path = this->GetKppCache3FilePath(dirName, k1, p1);

	if (!PathFileExistsA((LPCSTR)catch3Path.c_str()))
	{
		// �t�@�C����������Ȃ��̂� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
		return false;
	}

	std::ifstream ifs(catch3Path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//�Ȃ񂩏I������������
		return false;
	}

	//�ǂݍ��ރt�@�C���i�[�p�z��
	int p2 = 0;
	int z = 0;
	char buffer[2];
	while (!ifs.eof()) {

		ifs.read((char*)buffer, sizeof(s16));//2byte

		kppArray[k1][p1][p2][z] = *buffer;

		// �C���N�������g�B
		z++;
		if (z == 2) {
			z = 0;
			p2++;// �J��オ��
			if (p2 == fe_end) {
				break;
			}
		}
	}

	return true;
#else
	return false;
#endif
}

bool Evaluater::SynthesizeKppCache3ToCache2(const std::string & dirName, int k1)
{
#if !defined(SKIP_KPP_EVAL_LOOP) // KPP�]���l�t�@�C���̍쐬���X�L�b�v����ݒ�Ȃ�A���̊֐��͎��s���܂���B
#if defined(MODE_CACHE_EVAL)
	// Cache2�t�@�C���̗L���𒲂ׂ܂��B
	std::string cache2Path = this->GetKppCache2FilePath(dirName, k1);
	if (PathFileExistsA((LPCSTR)cache2Path.c_str()))
	{
		// �t�@�C�������������ꍇ�� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
		return false;
	}
	SYNCCOUT << "(Synthesize 1/3) Go KPP!: [" << cache2Path.c_str() << "]" << SYNCENDL;

	// Cache3�t�@�C���� 1548���邩���ׂ܂��B
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetKppCache3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			// �t�@�C����������Ȃ��̂� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
			return false;
		}
	}
	SYNCCOUT << "(Synthesize 2/3) [" << std::to_string(fe_end) << "] files exist. ok." << SYNCENDL;

	// �����o����
	std::ofstream output(cache2Path.c_str(), std::ios::binary);

	// Cache3�t�@�C���� 1548�ǂݍ��݂܂��B
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetKppCache3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			std::cerr << "Error : File not found. " << cache3Path.c_str() << std::endl;
			//�Ȃ񂩏I������������
			return false;
		}

		std::ifstream input(cache3Path.c_str(), std::ios::binary);
		if (input.fail()) {
			std::cerr << "Error : Could not open" << std::endl;
			//�Ȃ񂩏I������������
			return false;
		}

		int p2 = 0;
		int z = 0;
		char buffer[2];
		for (;;){// while (!input.eof()) {

			// �Ǎ�
			input.read((char*)buffer, sizeof(s16));// 2bytes

			// ���o
			output.write((char*)buffer, sizeof(s16));// 2bytes

			// �C���N�������g�B
			z++;
			if (z == 2) {
				z = 0;
				p2++;// �J��オ��
				if (p2 == fe_end) {
					break;
				}
			}
		}
	}
	// (^q^)SYNCCOUT �Ŏn�߂���ASYNCENDL �ŏI��邱�ƁB
	SYNCCOUT << "(Synthesize 3/3) (^q^)File synthesized!" << SYNCENDL;
	//�~ SYNCCOUT << "(Synthesize 3/3) (^q^)File synthesized!" << std::endl;

	return true;
#else
	return false;
#endif
#else
	return false;
#endif
}

std::string Evaluater::GetKppCache2FilePath(const std::string & dirName, int k1)
{
	std::string dir1 = addSlashIfNone(dirName) + "obj";
	std::string file2 = dir1 + "/Kpp[" + std::to_string(k1) + "][All].obj";
	return file2;
}

std::string Evaluater::GetKppCache3FilePath(const std::string & dirName, int k1, int p1)
{
	//SYNCCOUT << "(Write) File Search: dirName=[" << dirName << "]" << SYNCENDL;

	std::string dir1 = addSlashIfNone(dirName) + "obj";
	//SYNCCOUT << "(Write) File Search: dir1=[" << dir1 << "]" << SYNCENDL;

	std::string dir2 = dir1 + "/Kpp[" + std::to_string(k1) + "]";
	//SYNCCOUT << "(Write) File Search: dir2=[" << dir2 << "]" << SYNCENDL;

	std::string file3 = dir2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";
	//SYNCCOUT << "(Write) File Search: path3=[" << file3 << "]" << SYNCENDL;

	return file3;
}