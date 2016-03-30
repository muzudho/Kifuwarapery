#include "../n080_common__/n080_200_utilProgram.hpp"
#include "h260_500_Evaluater.hpp"
#include <Windows.h>	// ファイル／ディレクトリ操作用
#include <shlwapi.h>	// ファイル／ディレクトリ操作用 shlwapi.lib へのリンクが必要。


// KPPのファイル分割
bool Evaluater::WriteKppCache3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	bool isError = false;

#if !defined(SKIP_USE_KPP_CACHE)
	//SYNCCOUT << "(Write) File Search: dirName=[" << dirName << "]" << SYNCENDL;

	std::string dir1 = addSlashIfNone(dirName) + "obj";
	//SYNCCOUT << "(Write) File Search: dir1=[" << dir1 << "]" << SYNCENDL;

	std::string dir2 = dir1 + "/Kpp[" + std::to_string(k1) + "]";
	//SYNCCOUT << "(Write) File Search: dir2=[" << dir2 << "]" << SYNCENDL;

	std::string file3 = dir2 + "/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";

#if defined(MODE_EVAL)
	SYNCCOUT << "(WriteKppCache3Files 1/9) File Search: path3=[" << file3 << "]" << SYNCENDL;
#endif

	if (!PathIsDirectoryA((LPCSTR)dir1.c_str()))
	{
#if defined(MODE_EVAL)
		SYNCCOUT << "(WriteKppCache3Files 2/9)Not found directory : dir1=[" << dir1 << "]" << SYNCENDL;
#endif
		UtilProgram utilProgram;
		utilProgram.ErrorBegin();
		SetLastError(NO_ERROR);		//エラー情報をクリアする
		int result = CreateDirectoryA((LPCSTR)dir1.c_str(), NULL);
		utilProgram.ErrorEnd();

		if (result !=0)
		{
#if defined(MODE_EVAL)
			SYNCCOUT << "(WriteKppCache3Files 3/9)Create directory : dir1=[" << dir1 << "]" << SYNCENDL;
#endif
		}
		else
		{
			// エラー
			isError = true;
			std::cerr << "(WriteKppCache3Files 4/9): Can not create directory : dir1=[" << dir1 << "]" << SYNCENDL;
		}
	}

	if (!isError && !PathIsDirectoryA((LPCSTR)dir2.c_str()))
	{
		//SYNCCOUT << "Not found directory : path2=[" << dir2 << "]" << SYNCENDL;
		if (CreateDirectoryA((LPCSTR)dir2.c_str(), NULL))
		{
#if defined(MODE_EVAL)
			SYNCCOUT << "(WriteKppCache3Files 5/9)Create directory : dir2=[" << dir2 << "]" << SYNCENDL;
#endif
		}
		else
		{
			// エラー
			isError = true;
			std::cerr << "(WriteKppCache3Files 6/9)error: Can not create directory : dir2=[" << dir2 << "]" << SYNCENDL;
		}
	}

#if defined(MODE_EVAL)
	SYNCCOUT << "(WriteKppCache3Files 7/9)" << SYNCENDL;
#endif

	// ファイルは無いはず。
	if (!isError)
	{
		this->WriteKppCache3FilesBody(dirName, k1, p1, kppArray);
#if defined(MODE_EVAL)
		SYNCCOUT << "(WriteKppCache3Files 8/9)" << SYNCENDL;
#endif
	}
#endif

#if defined(MODE_EVAL)
	SYNCCOUT << "(WriteKppCache3Files 9/9)" << SYNCENDL;
#endif
	return !isError;
}

void Evaluater::WriteKppCache3FilesBody(const std::string & cache3Filepath, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
	std::ofstream output(cache3Filepath, std::ios::binary);

	//書き込むファイル格納用配列
	char buffer[2];
	int p2 = 0;
	int z = 0;
	for (;;) {

		buffer[0] = kppArray[k1][p1][p2][0];// 2byte * 1548element = 3096byte
		buffer[1] = kppArray[k1][p1][p2][1];// 2byte * 1548element = 3096byte
		output.write((char *)buffer, sizeof(s16));// 2element * 2byte * 1548element = 6192byte
												  
		z++;// インクリメント。
		if (z == 2) {
			z = 0;
			p2++;// 繰り上がり
			if (p2 == fe_end) {
				break;
			}
		}
	}
	//SYNCCOUT << "(^q^)Outputed!" << SYNCENDL;
}

bool Evaluater::ReadKppCache2Files(const std::string & dirName, int k1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if !defined(SKIP_USE_KPP_CACHE)
	std::string cache2Path = this->GetKppCache2FilePath(dirName, k1);

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

bool Evaluater::ReadKppCache3Files(const std::string & dirName, int k1, int p1, std::array<s16, 2> kppArray[SquareNum][fe_end][fe_end])
{
#if !defined(SKIP_USE_KPP_CACHE)
	std::string catch3Path = this->GetKppCache3FilePath(dirName, k1, p1);// addSlashIfNone(dirName) + "obj/Kpp[" + std::to_string(k1) + "]/Kpp[" + std::to_string(k1) + "][" + std::to_string(p1) + "].obj";

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

bool Evaluater::SynthesizeKppCache3ToCache2(const std::string & dirName, int k1)
{
#if !defined(SKIP_NOT_EXIST_EVAL_FILE) // 評価値ファイルが無かった場合スキップする設定なら、この関数は実行しません。
#if !defined(SKIP_USE_KPP_CACHE)
	// Cache2ファイルの有無を調べます。
	std::string cache2Path = this->GetKppCache2FilePath(dirName, k1);
	if (PathFileExistsA((LPCSTR)cache2Path.c_str()))
	{
		// ファイルが見つかった場合は OK です。falseを返して正常終了します。
		return false;
	}
#if defined(MODE_EVAL)
	SYNCCOUT << "(Synthesize 1/3) Go KPP!: [" << cache2Path << "]" << SYNCENDL;
#endif

	// Cache3ファイルが 1548個あるか調べます。
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetKppCache3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			// ファイルが見つからないのは OK です。falseを返して正常終了します。
			return false;
		}
	}
#if defined(MODE_EVAL)
	SYNCCOUT << "(Synthesize 2/3) [" << fe_end << "] files exist. ok." << SYNCENDL;
#endif

	// 書き出し先
	std::ofstream output(cache2Path.c_str(), std::ios::binary);

	// Cache3ファイルを 1548個読み込みます。
	for (int p1 = 0; p1 < fe_end; ++p1) {
		std::string cache3Path = this->GetKppCache3FilePath(dirName, k1, p1);

		if (!PathFileExistsA((LPCSTR)cache3Path.c_str()))
		{
			std::cerr << "Error : File not found. " << cache3Path << std::endl;
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
		for (;;){// while (!input.eof()) {

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

		// よく強制終了するので、明示的に閉じて見ます。
		//input.close();
	}
#if defined(MODE_EVAL)
	SYNCCOUT << "(Synthesize 3/3) (^q^)File synthesized!" << std::endl;
#endif

	// よく強制終了するので、明示的に閉じて見ます。
	//output.close();

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
