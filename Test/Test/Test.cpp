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
	setting.add(ENCRYPT(L"mg{3"), ENCRYPT(L"xcnwg3"));
	setting.add(ENCRYPT(L"mg{4"), ENCRYPT(L"xcnwg4"));
	setting.add(ENCRYPT(L"mg{5"), ENCRYPT(L"xcnwg5"));
	

	std::wcout << setting.get(ENCRYPT(L"mg{3")) << setting.get(ENCRYPT(L" ")) << setting.getMap(ENCRYPT(L"mg{3")) << std::endl; // output: "value1"
	std::wcout << setting.get(ENCRYPT(L"mg{4")) << setting.get(ENCRYPT(L" ")) << setting.getMap(ENCRYPT(L"mg{4")) << std::endl; // output: "value2"
	std::wcout << setting.get(ENCRYPT(L"mg{5")) << setting.get(ENCRYPT(L" ")) << setting.getMap(ENCRYPT(L"mg{5")) << std::endl << std::endl; // output: "value3"

	CSetting2 setting2(&setting);

	std::wcout << setting.get(ENCRYPT(L"mg{6")) << setting.get(ENCRYPT(L" ")) << setting2.get(ENCRYPT(L"mg{6")) << std::endl; // output: "value4"
	std::wcout << setting.get(ENCRYPT(L"mg{7")) << setting.get(ENCRYPT(L" ")) << setting2.get(ENCRYPT(L"mg{7")) << std::endl; // output: "value5"
	std::wcout << setting.get(ENCRYPT(L"mg{8")) << setting.get(ENCRYPT(L" ")) << setting2.get(ENCRYPT(L"mg{8")) << std::endl; // output: "value6"


	std::system("PAUSE");
	return nRetCode;
}
