//#include "stdafx.h"
#include "h260_500_Evaluater.hpp"
#include <Windows.h>	// �t�@�C���^�f�B���N�g������p
#include <shlwapi.h>	// �t�@�C���^�f�B���N�g������p shlwapi.lib �ւ̃����N���K�v�B

// KPP�̃t�@�C������
void Evaluater::WriteKppPartFile(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if !defined(SKIP_KPP_PART_WRITE)
	//SYNCCOUT << "(Write) File Search: dirName=[" << dirName << "]" << SYNCENDL;

	std::string dir1 = addSlashIfNone(dirName) + "obj";
	//SYNCCOUT << "(Write) File Search: dir1=[" << dir1 << "]" << SYNCENDL;

	std::string dir2 = dir1 + "/Kpp[" + std::to_string(k1) + "]";
	//SYNCCOUT << "(Write) File Search: dir2=[" << dir2 << "]" << SYNCENDL;

	std::string file3 = dir2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";

	//std::string file3 = this->GetKppPartFilePath(dirName, k1, p1);
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

	std::ofstream ofs(file3.c_str(), std::ios::binary);
	//SYNCCOUT << "(^q^)Go! KPP!" << SYNCENDL;

	//�������ރt�@�C���i�[�p�z��
	int p2 = 0;
	int z = 0;
	for (;;) {//while (!ofs.eof()) {

		//ofs << kppArray[k1][p1][p2][z];
		ofs << (short)kppArray[k1][p1][p2][z];

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

bool Evaluater::ReadKppPartFile(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	std::string path = this->GetKppPartFilePath(dirName, k1, p1);// addSlashIfNone(dirName) + "obj/Kpp[" + std::to_string(k1) + "]/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";
	SYNCCOUT << "(Read) File Search: [" << path << "]" << SYNCENDL;

	if (!PathFileExistsA((LPCSTR)path.c_str()))
	{
		std::cerr << "File not found (ok)." << std::endl;
		return false;
	}
	std::cerr << "(^q^)File found." << std::endl;

	std::ifstream ifs(path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//�Ȃ񂩏I������������
		return false;
	}
	std::cerr << "(^q^)File opened." << std::endl;

	//�ǂݍ��ރt�@�C���i�[�p�z��
	int p2 = 0;
	int z = 0;
	s16 *buffer = 0;
	while (!ifs.eof()) {

		ifs.read((char*)buffer, sizeof(short));

		std::cerr << "(^q^)Skip! k1=[" << std::to_string(k1) << "] p1=[" << std::to_string(p1) << "] p2=[" << std::to_string(p2) << "] z=[" << std::to_string(z) << "]" << std::endl;

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
}

std::string Evaluater::GetKppPartFilePath(const std::string & dirName, int k1, int p1)
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
