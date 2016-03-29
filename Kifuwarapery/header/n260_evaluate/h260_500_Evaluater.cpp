//#include "stdafx.h"
#include "h260_500_Evaluater.hpp"
#include <Windows.h>	// ファイル／ディレクトリ操作用
#include <shlwapi.h>	// ファイル／ディレクトリ操作用 shlwapi.lib へのリンクが必要。

void Evaluater::WriteKppPartFile2(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	SYNCCOUT << "(Write) File Search: dirName=[" << dirName << "]" << SYNCENDL;

	std::string path1 = addSlashIfNone(dirName) + "obj";
	SYNCCOUT << "(Write) File Search: path1=[" << path1 << "]" << SYNCENDL;

	std::string path2 = path1 + "/Kpp[" + std::to_string(k1) + "]";
	SYNCCOUT << "(Write) File Search: path2=[" << path2 << "]" << SYNCENDL;

	std::string path3 = path2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";
	SYNCCOUT << "(Write) File Search: path3=[" << path3 << "]" << SYNCENDL;


	if (PathIsDirectoryW((LPCWSTR)path1.c_str()))
	{
		CreateDirectoryW((LPCWSTR)path1.c_str(), NULL);
	}

	if (PathIsDirectoryW((LPCWSTR)path2.c_str()))
	{
		CreateDirectoryW((LPCWSTR)path2.c_str(), NULL);
	}

	if (PathIsDirectoryW((LPCWSTR)path3.c_str()))
	{
		CreateDirectoryW((LPCWSTR)path3.c_str(), NULL);
	}

	std::ofstream ofs(path3.c_str(), std::ios::binary);

	//書き込むファイル格納用配列
	int p01 = 0;
	int p02 = 0;
	int z = 0;
	while (!ofs.eof()) {

		ofs << (char*)kppArray[k1][p01][p02][z];

		// インクリメント。
		z++;
		if (z == 2) {
			z = 0;
			p02++;// 繰り上がり
			if (p02 == fe_end) {
				p02 = 0;
				p01++;// 繰り上がり
				if (p01 == fe_end) {
					break;
				}
			}
		}
	}
}

bool Evaluater::ReadKppPartFile2(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	std::string path = addSlashIfNone(dirName) + "obj/Kpp[" + std::to_string(k1) + "]/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";
	SYNCCOUT << "(Read) File Search: [" << path << "]" << SYNCENDL;

	if (PathFileExists((LPCWSTR)path.c_str()))
	{
		std::cerr << "File not found (ok)." << std::endl;
		return false;
	}

	std::ifstream ifs(path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//なんか終了処理を入れる
		return false;
	}

	//読み込むファイル格納用配列
	int p01 = 0;
	int p02 = 0;
	int z = 0;
	s16 *buffer = 0;
	while (!ifs.eof()) {
		ifs.read((char*)buffer, sizeof(short));

		kppArray[k1][p01][p02][z] = *buffer;

		// インクリメント。
		z++;
		if (z == 2) {
			z = 0;
			p02++;// 繰り上がり
			if (p02 == fe_end) {
				p02 = 0;
				p01++;// 繰り上がり
				if (p01 == fe_end) {
					break;
				}
			}
		}

	}
	return true;
}
