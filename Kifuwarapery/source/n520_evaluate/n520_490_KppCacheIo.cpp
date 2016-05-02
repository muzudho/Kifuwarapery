//#include <Windows.h>	// ファイル／ディレクトリ操作用
//#include <shlwapi.h>	// ファイル／ディレクトリ操作用 shlwapi.lib へのリンクが必要。
#include "../../header/n080_common__/n080_200_utilProgram.hpp"
#include "../../header/n520_evaluate/n520_490_KppCacheIo.hpp"
#include "../../header/n520_evaluate/n520_500_kkKkpKppStorage1.hpp"
#include <Windows.h>	// ファイル／ディレクトリ操作用
#include <shlwapi.h>	// ファイル／ディレクトリ操作用 shlwapi.lib へのリンクが必要。


bool KppCacheIo::SynthesizeLv3To2(const std::string & dirName, int k1)
{
#if !defined(SKIP_KPP_EVAL_LOOP) // KPP評価値ファイルの作成をスキップする設定なら、この関数は実行しません。
#if defined(MODE_CACHE_EVAL)
	// Cache2ファイルの有無を調べます。
	std::string cache2Path = this->GetLv2FilePath(dirName, k1);
	if (PathFileExistsA((LPCSTR)cache2Path.c_str()))
	{
		// ファイルが見つかった場合は OK です。falseを返して正常終了します。
		return false;
	}
	SYNCCOUT << "(Synthesize 1/3) Go KPP!: [" << cache2Path.c_str() << "]" << SYNCENDL;

	// Cache3ファイルが 1548個あるか調べます。
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetLv3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			// ファイルが見つからないのは OK です。falseを返して正常終了します。
			return false;
		}
	}
	SYNCCOUT << "(Synthesize 2/3) [" << std::to_string(fe_end) << "] files exist. ok." << SYNCENDL;

	// 書き出し先
	std::ofstream output(cache2Path.c_str(), std::ios::binary);

	// Cache3ファイルを 1548個読み込みます。
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetLv3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			std::cerr << "Error : File not found. " << cache3Path.c_str() << std::endl;
			//なんか終了処理を入れる
			return false;
		}

		std::ifstream input(cache3Path.c_str(), std::ios::binary);
		if (input.fail()) {
			std::cerr << "Error : Could not open" << std::endl;
			//なんか終了処理を入れる
			return false;
		}

		int p2 = 0;
		int z = 0;
		char buffer[2];
		for (;;) {// while (!input.eof()) {

				  // 読込
			input.read((char*)buffer, sizeof(s16));// 2bytes

												   // 書出
			output.write((char*)buffer, sizeof(s16));// 2bytes

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
	}
	// (^q^)SYNCCOUT で始めたら、SYNCENDL で終わること。
	SYNCCOUT << "(Synthesize 3/3) (^q^)File synthesized!" << SYNCENDL;
	//× SYNCCOUT << "(Synthesize 3/3) (^q^)File synthesized!" << std::endl;

	return true;
#else
	return false;
#endif
#else
	return false;
#endif
}

// KPPのファイル分割
bool KppCacheIo::WriteLv3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	bool isError = false;

#if defined(MODE_CACHE_EVAL)

	std::string dir1 = KkKkpKppStorage1::AppendSlashIfNone(dirName) + "obj";
	std::string dir2 = dir1 + "/Kpp[" + std::to_string(k1) + "]";
	std::string file3 = dir2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";

	//SYNCCOUT << "(WriteKppCache3Files 1/9) File: path3=[" << file3.c_str() << "]" << SYNCENDL;

	// obj ディレクトリがなければ作ります。
	if (!PathIsDirectoryA((LPCSTR)dir1.c_str()))
	{
		// ディレクトリーがないというのは正常動作です。
		//SYNCCOUT << "(WriteKppCache3Files 2/9)Not found directory. ok. : dir1=[" << dir1.c_str() << "]" << SYNCENDL;

		UtilProgram utilProgram;
		utilProgram.ErrorBegin();
		int result = CreateDirectoryA((LPCSTR)dir1.c_str(), NULL);
		utilProgram.ErrorEnd();

		if (result != 0)
		{
			//SYNCCOUT << "(WriteKppCache3Files 3/9)Create directory : dir1=[" << dir1.c_str() << "]" << SYNCENDL;
		}
		else
		{
			// エラー
			isError = true;

			std::cerr << "(WriteKppCache3Files 4/9 error): Can not create directory : dir1=[" << dir1.c_str() << "]" << std::endl;
			utilProgram.ShowCurrentDirectory();
		}
	}

	// Kpp[数字]ディレクトリがなければ作ります。
	if (!isError && !PathIsDirectoryA((LPCSTR)dir2.c_str()))
	{
		//SYNCCOUT << "Not found directory : path2=[" << dir2 << "]" << SYNCENDL;
		if (CreateDirectoryA((LPCSTR)dir2.c_str(), NULL))
		{
			//SYNCCOUT << "(WriteKppCache3Files 5/9)Create directory : dir2=[" << dir2.c_str() << "]" << SYNCENDL;
		}
		else
		{
			// エラー
			isError = true;
			std::cerr << "(WriteKppCache3Files 6/9 error) Can not create directory : dir2=[" << dir2.c_str() << "]" << std::endl;
		}
	}

	//SYNCCOUT << "(WriteKppCache3Files 7/9)" << SYNCENDL;

	// ファイルは無いはず。
	if (!isError)
	{
		this->WriteLv3FilesBody(file3, k1, p1, kppArray);
		//SYNCCOUT << "(WriteKppCache3Files 8/9)" << SYNCENDL;
	}
	//SYNCCOUT << "(WriteKppCache3Files 9/9)" << SYNCENDL;
#endif
	return !isError;
}

void KppCacheIo::WriteLv3FilesBody(const std::string & cache3Filepath, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	std::ofstream output(cache3Filepath, std::ios::binary);

	//書き込むファイル格納用配列
	// 2byte * 1548element = 3096byte
	// 2element * 2byte * 1548element = 6192byte
	char buffer[2];
	int p2 = 0;
	int z = 0;
	for (;;) {

		// short型を、char型２つに分割します。
		/*
		buffer[0] = (char)(kppArray[k1][p1][p2][z] & 0xff);// 8bits
		buffer[1] = (char)(kppArray[k1][p1][p2][z] >> 8 & 0xff);// 8bits
		*/
		buffer[0] = (char)(kppArray[k1][p1][p2][z] >> 8 & 0xff);// 8bits
		buffer[1] = (char)(kppArray[k1][p1][p2][z] & 0xff);// 8bits
		output.write((char *)buffer, sizeof(s16));// 16bits

		z++;// インクリメント。
		if (z == 2) {
			z = 0;
			p2++;// 繰り上がり
			if (p2 == fe_end) {
				break;
			}
		}
	}
}

bool KppCacheIo::ReadLv2Files(const std::string & dirName, int k1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if defined(MODE_CACHE_EVAL)
	KppCacheIo kppCacheIo;
	std::string cache2Path = kppCacheIo.GetLv2FilePath(dirName, k1);

	if (!PathFileExistsA((LPCSTR)cache2Path.c_str()))
	{
		// ファイルが見つからないのは OK です。falseを返して正常終了します。
		return false;
	}

	std::ifstream ifs(cache2Path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//なんか終了処理を入れる
		return false;
	}
	//SYNCCOUT << "(^q^)File opened." << std::endl;

	//読み込むファイル格納用配列
	int p1 = 0;
	int p2 = 0;
	int z = 0;
	char buffer[2]; //s16 *buffer = 0;
	while (!ifs.eof()) {

		ifs.read((char*)buffer, sizeof(s16));//2byte

		kppArray[k1][p1][p2][z] = *buffer;

		// インクリメント。
		z++;
		if (z == 2) {
			z = 0;
			p2++;// 繰り上がり
			if (p2 == fe_end) {
				p2 = 0;
				p1++;// 繰り上がり
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

bool KppCacheIo::ReadLv3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if defined(MODE_CACHE_EVAL)
	KppCacheIo kppCacheIo;
	std::string catch3Path = kppCacheIo.GetLv3FilePath(dirName, k1, p1);

	if (!PathFileExistsA((LPCSTR)catch3Path.c_str()))
	{
		// ファイルが見つからないのは OK です。falseを返して正常終了します。
		return false;
	}

	std::ifstream ifs(catch3Path.c_str(), std::ios::binary);
	if (ifs.fail()) {
		std::cerr << "Error : Could not open" << std::endl;
		//なんか終了処理を入れる
		return false;
	}

	//読み込むファイル格納用配列
	int p2 = 0;
	int z = 0;
	char buffer[2];
	while (!ifs.eof()) {

		ifs.read((char*)buffer, sizeof(s16));//2byte

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

	return true;
#else
	return false;
#endif
}

std::string KppCacheIo::GetLv2FilePath(const std::string & dirName, int k1)
{
	std::string dir1 = KkKkpKppStorage1::AppendSlashIfNone(dirName) + "obj";
	std::string file2 = dir1 + "/Kpp[" + std::to_string(k1) + "][All].obj";
	return file2;
}

std::string KppCacheIo::GetLv3FilePath(const std::string & dirName, int k1, int p1)
{
	//SYNCCOUT << "(Write) File: dirName=[" << dirName << "]" << SYNCENDL;

	std::string dir1 = KkKkpKppStorage1::AppendSlashIfNone(dirName) + "obj";
	//SYNCCOUT << "(Write) File: dir1=[" << dir1 << "]" << SYNCENDL;

	std::string dir2 = dir1 + "/Kpp[" + std::to_string(k1) + "]";
	//SYNCCOUT << "(Write) File: dir2=[" << dir2 << "]" << SYNCENDL;

	std::string file3 = dir2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";
	//SYNCCOUT << "(Write) File: path3=[" << file3 << "]" << SYNCENDL;

	return file3;
}
