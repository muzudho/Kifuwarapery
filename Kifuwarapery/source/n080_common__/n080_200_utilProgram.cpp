#include <windows.h> // ヘッダー・ファイルに書くとエラーになる。
#include "../../header/n080_common__/n080_090_syncCout.hpp"
#include "../../header/n080_common__/n080_100_common.hpp"
#include "..\..\header\n080_common__\n080_200_utilProgram.hpp"


void UtilProgram::ErrorBegin()
{
	SetLastError(NO_ERROR);		//エラー情報をクリアする
}

void UtilProgram::ErrorEnd()
{
	LPVOID lpMsgBuf;
	FormatMessage(				//エラー表示文字列作成
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (const char*)lpMsgBuf, NULL, MB_OK);	//メッセージ表示
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, NULL, MB_OK);	//メッセージ表示
	LocalFree(lpMsgBuf);
}

void UtilProgram::ShowCurrentDirectory()
{
	TCHAR crDir[MAX_PATH + 1];
	GetCurrentDirectory(MAX_PATH + 1, crDir);
	crDir[MAX_PATH] = '\0';
	SYNCCOUT << "Current directory=[" << SYNCENDL;
	for (int i = 0; crDir[i] != '\0'; i++)
	{
		SYNCCOUT << ((char)crDir[i]) << SYNCENDL;
	}
	SYNCCOUT << "]" << SYNCENDL;
}
