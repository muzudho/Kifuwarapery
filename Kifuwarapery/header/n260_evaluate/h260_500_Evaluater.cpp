//#include "stdafx.h"
#include "h260_500_Evaluater.hpp"
#include <Windows.h>	// ファイル／ディレクトリ操作用
#include <shlwapi.h>	// ファイル／ディレクトリ操作用 shlwapi.lib へのリンクが必要。

// KPPのファイル分割
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

	// ファイルは無いはず。

	std::ofstream ofs(file3.c_str(), std::ios::binary);
	//SYNCCOUT << "(^q^)Go! KPP!" << SYNCENDL;

	//書き込むファイル格納用配列
	int p2 = 0;
	int z = 0;
	for (;;) {//while (!ofs.eof()) {

		//ofs << kppArray[k1][p1][p2][z];
		ofs << (short)kppArray[k1][p1][p2][z];

		// インクリメント。z++にするとなぜか強制終了する。
		z++;

		if (z == 2) {
			z = 0;
			p2++;// 繰り上がり
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
		//なんか終了処理を入れる
		return false;
	}
	std::cerr << "(^q^)File opened." << std::endl;

	//読み込むファイル格納用配列
	int p2 = 0;
	int z = 0;
	s16 *buffer = 0;
	while (!ifs.eof()) {

		ifs.read((char*)buffer, sizeof(short));

		std::cerr << "(^q^)Skip! k1=[" << std::to_string(k1) << "] p1=[" << std::to_string(p1) << "] p2=[" << std::to_string(p2) << "] z=[" << std::to_string(z) << "]" << std::endl;

		kppArray[k1][p1][p2][z] = *buffer;

		// インクリメント。
		z++;
		if (z == 2) {
			z = 0;
			p2++;// 繰り上がり
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
