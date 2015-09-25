// Test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Test.h"
#include "Setting.h"
#include "Other_codes\Setting2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define ENCRYPT(x) x

// The one and only application object

CWinApp theApp;


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	HMODULE hModule = ::GetModuleHandle(NULL);

	if (hModule != NULL)
	{
		// initialize MFC and print and error on failure
		if (!AfxWinInit(hModule, NULL, ::GetCommandLine(), 0))
		{
			// TODO: change error code to suit your needs
			_tprintf(_T("Fatal Error: MFC initialization failed\n"));
			nRetCode = 1;
		}
		else
		{
			// TODO: code your application's behavior here.
			goto m1;
		}
	}
	else
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: GetModuleHandle failed\n"));
		nRetCode = 1;
	}

	m1:

	CSetting setting;
	setting.add(ENCRYPT(L"key1"), ENCRYPT(L"value1"));
	setting.add(ENCRYPT(L"key2"), ENCRYPT(L"value2"));
	setting.add(ENCRYPT(L"key3"), ENCRYPT(L"value3"));
	

	std::wcout << setting.get(ENCRYPT(L"key1")) << setting.get(ENCRYPT(L" ")) << setting.getMap(ENCRYPT(L"key1")) << std::endl; // output: "value1"
	std::wcout << setting.get(ENCRYPT(L"key2")) << setting.get(ENCRYPT(L" ")) << setting.getMap(ENCRYPT(L"key2")) << std::endl; // output: "value2"
	std::wcout << setting.get(ENCRYPT(L"key3")) << setting.get(ENCRYPT(L" ")) << setting.getMap(ENCRYPT(L"key3")) << std::endl << std::endl; // output: "value3"

	CSetting2 setting2(&setting);

	std::wcout << setting.get(ENCRYPT(L"key4")) << setting.get(ENCRYPT(L" ")) << setting2.get(ENCRYPT(L"key4")) << std::endl; // output: "value4"
	std::wcout << setting.get(ENCRYPT(L"key5")) << setting.get(ENCRYPT(L" ")) << setting2.get(ENCRYPT(L"key5")) << std::endl; // output: "value5"
	std::wcout << setting.get(ENCRYPT(L"key6")) << setting.get(ENCRYPT(L" ")) << setting2.get(ENCRYPT(L"key6")) << std::endl; // output: "value6"


	std::system("PAUSE");
	return nRetCode;
}
