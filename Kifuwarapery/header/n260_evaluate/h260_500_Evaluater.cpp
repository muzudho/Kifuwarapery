//#include "stdafx.h"
#include "h260_500_Evaluater.hpp"
#include <Windows.h>	// �t�@�C���^�f�B���N�g������p
#include <shlwapi.h>	// �t�@�C���^�f�B���N�g������p shlwapi.lib �ւ̃����N���K�v�B

// KPP�̃t�@�C������
void Evaluater::WriteKppCache3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if defined(USE_KPP_CACHE)
	//SYNCCOUT << "(Write) File Search: dirName=[" << dirName << "]" << SYNCENDL;

	std::string dir1 = addSlashIfNone(dirName) + "obj";
	//SYNCCOUT << "(Write) File Search: dir1=[" << dir1 << "]" << SYNCENDL;

	std::string dir2 = dir1 + "/Kpp[" + std::to_string(k1) + "]";
	//SYNCCOUT << "(Write) File Search: dir2=[" << dir2 << "]" << SYNCENDL;

	std::string file3 = dir2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";

	//std::string file3 = this->GetKppCache3FilePath(dirName, k1, p1);
	SYNCCOUT << "(Write) File Search: path3=[" << file3 << "]" << SYNCENDL;

	if (!PathIsDirectoryA((LPCSTR)dir1.c_str()))
	{
		//SYNCCOUT << "Not found directory : path1=[" << dir1 << "]" << SYNCENDL;
		if (CreateDirectoryA((LPCSTR)dir1.c_str(), NULL))
		{
			SYNCCOUT << "Create directory : dir1=[" << dir1 << "]" << SYNCENDL;
		}
		else
		{
			SYNCCOUT << "Can not create directory : dir1=[" << dir1 << "]" << SYNCENDL;
		}
	}

	if (!PathIsDirectoryA((LPCSTR)dir2.c_str()))
	{
		//SYNCCOUT << "Not found directory : path2=[" << dir2 << "]" << SYNCENDL;
		if (CreateDirectoryA((LPCSTR)dir2.c_str(), NULL))
		{
			SYNCCOUT << "Create directory : dir2=[" << dir2 << "]" << SYNCENDL;
		}
		else
		{
			SYNCCOUT << "Can not create directory : dir2=[" << dir2 << "]" << SYNCENDL;
		}
	}

	// �t�@�C���͖����͂��B

	std::ofstream output(file3.c_str(), std::ios::binary);
	//SYNCCOUT << "(^q^)Go! KPP!" << SYNCENDL;

	//�������ރt�@�C���i�[�p�z��
	char buffer[2];
	int p2 = 0;
	int z = 0;
	for (;;) {//while (!output.eof()) {

		////output << kppArray[k1][p1][p2][z];
		//output << (short)kppArray[k1][p1][p2][z];
		buffer[0] = kppArray[k1][p1][p2][0];// 2byte * 1548element = 3096byte
		buffer[1] = kppArray[k1][p1][p2][1];// 2byte * 1548element = 3096byte
		//output.write((char *)buffer, sizeof(short));// 2element * 2byte * 1548element = 6192byte
		output.write((char *)buffer, sizeof(s16));// 2element * 2byte * 1548element = 6192byte

		// �C���N�������g�Bz++�ɂ���ƂȂ��������I������B
		z++;

		if (z == 2) {
			z = 0;
			p2++;// �J��オ��
			if (p2 == fe_end) {
				break;
			}
		}
	}
	//SYNCCOUT << "(^q^)Outputed!" << SYNCENDL;
#endif
}

bool Evaluater::ReadKppCache2Files(const std::string & dirName, int k1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if defined(USE_KPP_CACHE)
	std::string path = this->GetKppCache2FilePath(dirName, k1);
	SYNCCOUT << "(Read) Cache2 Search: [" << path << "]" << SYNCENDL;

	if (!PathFileExistsA((LPCSTR)path.c_str()))
	{
		// �t�@�C����������Ȃ��̂� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
		return false;
	}

	std::ifstream ifs(path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//�Ȃ񂩏I������������
		return false;
	}
	//std::cerr << "(^q^)File opened." << std::endl;

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
	std::cerr << "(^q^)File loaded!" << std::endl;

	return true;
#else
	return false;
#endif
}

bool Evaluater::ReadKppCache3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if defined(USE_KPP_CACHE)
	std::string path = this->GetKppCache3FilePath(dirName, k1, p1);// addSlashIfNone(dirName) + "obj/Kpp[" + std::to_string(k1) + "]/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";
	SYNCCOUT << "(Read) File Search: [" << path << "]" << SYNCENDL;

	if (!PathFileExistsA((LPCSTR)path.c_str()))
	{
		// �t�@�C����������Ȃ��̂� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
		//std::cerr << "File not found (ok)." << std::endl;
		return false;
	}
	//std::cerr << "(^q^)File found." << std::endl;

	std::ifstream ifs(path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//�Ȃ񂩏I������������
		return false;
	}
	//std::cerr << "(^q^)File opened." << std::endl;

	//�ǂݍ��ރt�@�C���i�[�p�z��
	int p2 = 0;
	int z = 0;
	char buffer[2]; //s16 *buffer = 0;
	while (!ifs.eof()) {

		ifs.read((char*)buffer, sizeof(s16));//2byte

		//std::cerr << "(^q^)Skip! k1=[" << std::to_string(k1) << "] p1=[" << std::to_string(p1) << "] p2=[" << std::to_string(p2) << "] z=[" << std::to_string(z) << "]" << std::endl;

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
	std::cerr << "(^q^)File loaded!" << std::endl;

	return true;
#else
	return false;
#endif
}

bool Evaluater::SynthesizeKppCache3ToCache2(const std::string & dirName, int k1)
{
#if defined(USE_KPP_CACHE)
	// Cache2�t�@�C���̗L���𒲂ׂ܂��B
	std::string cache2Path = this->GetKppCache2FilePath(dirName, k1);
	if (PathFileExistsA((LPCSTR)cache2Path.c_str()))
	{
		// �t�@�C�������������ꍇ�� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
		return false;
	}
	SYNCCOUT << "(Synthesize) Go KPP!: [" << cache2Path << "]" << SYNCENDL;

	// Cache3�t�@�C���� 1548���邩���ׂ܂��B
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetKppCache3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			// �t�@�C����������Ȃ��̂� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
			return false;
		}
	}
	SYNCCOUT << "(Synthesize) [" << fe_end << "] files exist. ok." << SYNCENDL;

	// �����o����
	std::ofstream output(cache2Path.c_str(), std::ios::binary);

	// Cache3�t�@�C���� 1548�ǂݍ��݂܂��B
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetKppCache3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			// �t�@�C����������Ȃ��̂� OK �ł��Bfalse��Ԃ��Đ���I�����܂��B
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
		while (!input.eof()) {

			// �Ǎ�
			input.read((char*)buffer, sizeof(s16));// 2bytes

			// ���o
			output.write((char *)buffer, sizeof(s16));// 2bytes

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
	std::cerr << "(^q^)File synthesized!" << std::endl;

	return true;
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
